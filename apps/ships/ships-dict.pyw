#!/usr/bin/env python

import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import ships

MAC = "qt_mac_set_native_menubar" in dir()



class MainForm(QDialog):

    def __init__(self, parent=None):
        super(MainForm, self).__init__(parent)

        listLabel = QLabel("&List")
        self.listWidget = QListWidget()
        listLabel.setBuddy(self.listWidget)

        tableLabel = QLabel("&Table")
        self.tableWidget = QTableWidget()
        tableLabel.setBuddy(self.tableWidget)

        treeLabel = QLabel("Tre&e")
        self.treeWidget = QTreeWidget()
        treeLabel.setBuddy(self.treeWidget)

        addShipButton = QPushButton("&Add Ship")
        removeShipButton = QPushButton("&Remove Ship")
        quitButton = QPushButton("&Quit")
        if not MAC:
            addShipButton.setFocusPolicy(Qt.NoFocus)
            removeShipButton.setFocusPolicy(Qt.NoFocus)
            quitButton.setFocusPolicy(Qt.NoFocus)

        splitter = QSplitter(Qt.Horizontal)

        vbox = QVBoxLayout()
        vbox.addWidget(listLabel)
        vbox.addWidget(self.listWidget)
        widget = QWidget()
        widget.setLayout(vbox)
        splitter.addWidget(widget)

        vbox = QVBoxLayout()
        vbox.addWidget(tableLabel)
        vbox.addWidget(self.tableWidget)
        widget = QWidget()
        widget.setLayout(vbox)
        splitter.addWidget(widget)

        vbox = QVBoxLayout()
        vbox.addWidget(treeLabel)
        vbox.addWidget(self.treeWidget)
        widget = QWidget()
        widget.setLayout(vbox)
        splitter.addWidget(widget)

        buttonLayout = QHBoxLayout()
        buttonLayout.addWidget(addShipButton)
        buttonLayout.addWidget(removeShipButton)
        buttonLayout.addStretch()
        buttonLayout.addWidget(quitButton)

        layout = QVBoxLayout()
        layout.addWidget(splitter)
        layout.addLayout(buttonLayout)
        self.setLayout(layout)

        self.connect(self.tableWidget, SIGNAL("itemChanged(QTableWidgetItem*)"), self.tableItemChanged)
        self.connect(addShipButton, SIGNAL("clicked()"), self.addShip)
        self.connect(removeShipButton, SIGNAL("clicked()"), self.removeShip)
        self.connect(quitButton, SIGNAL("clicked()"), self.accept)

        self.ships = ships.ShipContainer(QString("ships.dat"))

        self.setWindowTitle("Ships (dict)")
        QTimer.singleShot(0, self.initialLoad)
        return

    def initialLoad(self):
        if not QFile.exists(self.ships.filename):
            for ship in ships.generateFakeShips():
                self.ships.addShip(ship)
            self.ships.dirty = False
        else:
            try:
                self.ships.load()
            except IOError, e:
                QMessageBox.warning(self, "Ships - Error", "Failed to load: %s" % e)
        self.populateList()
        self.populateTable()
        self.tableWidget.sortItems(0)
        self.populateTree()

    def reject(self):
        self.accept()

    def accept(self):
        if self.ships.dirty and QMessageBox.question(self, "Ships - Save?",
                                                     "Save unsaved changes?",
                                                     QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes:
            try:
                self.ships.save()
            except IOError, e:
                QMessageBox.warning(self, "Ships - Error", "Failed to save: %s" % e)

        QDialog.accept(self)

    def populateList(self, selectedShip=None):
        selected = None
        self.listWidget.clear()
        for ship in self.ships.inOrder():
            item = QListWidgetItem(QString("%1 of %2/%3 (%L4)")
                                   .arg(ship.name)
                                   .arg(ship.owner)
                                   .arg(ship.country)
                                   .arg(ship.teu))
            self.listWidget.addItem(item)
            if (selectedShip is not None) and (selectedShip == id(ship)):
                selected = item
        if selected is not None:
            selected.setSelected(True)
            self.listWidget.setCurrentItem(selected)
        return

    def populateTable(self, selectedShip=None):
        selected = None
        self.tableWidget.clear()
        self.tableWidget.setSortingEnabled(False)
        self.tableWidget.setRowCount(len(self.ships))
        headers = ["Name", "Owner", "Country", "Description", "TEU"]
        self.tableWidget.setColumnCount(len(headers))
        self.tableWidget.setHorizontalHeaderLabels(headers)
        for row, ship in enumerate(self.ships):
            item = QTableWidgetItem(ship.name)
            item.setData(Qt.UserRole, QVariant(long(id(ship))))
            if (selectedShip is not None) and (selectedShip == id(ship)):
                selected = item
            self.tableWidget.setItem(row, ships.NAME, item)
            self.tableWidget.setItem(row, ships.OWNER, QTableWidgetItem(ship.owner))
            self.tableWidget.setItem(row, ships.COUNTRY, QTableWidgetItem(ship.country))
            self.tableWidget.setItem(row, ships.DESCRIPTION, QTableWidgetItem(ship.description))
            item = QTableWidgetItem(QString("%L1").arg(ship.teu, 8, 10, QChar(" ")))
            self.tableWidget.setItem(row, ships.TEU, item)
        self.tableWidget.setSortingEnabled(True)
        self.tableWidget.resizeColumnsToContents()
        if selected is not None:
            selected.setSelected(True)
            self.tableWidget.setCurrentItem(selected)
        return

    def populateTree(self, selectedShip=None):
        selected = None
        self.treeWidget.clear()
        self.treeWidget.setColumnCount(2)
        self.treeWidget.setHeaderLabels(["Country/Owner/Name", "TEU"])
        self.treeWidget.setItemsExpandable(True)

        parentFromCountry = dict()
        parentFromCountryOwner = dict()

        for ship in self.ships.inCountryOwnerOrder():
            
            ancestor = parentFromCountry.get(ship.country)
            if ancestor is None:
                ancestor = QTreeWidgetItem(self.treeWidget, [ship.country])
                parentFromCountry[ship.country] = ancestor
            countryowner = ship.country + "/" + ship.owner
            
            parent = parentFromCountryOwner.get(countryowner)
            if parent is None:
                parent = QTreeWidgetItem(ancestor, [ship.owner])
                parentFromCountryOwner[countryowner] = parent

            item = QTreeWidgetItem(parent, [ship.name, QString("%L1").arg(ship.teu)])
            item.setTextAlignment(1, Qt.AlignRight|Qt.AlignVCenter)
            if (selectedShip is not None) and (selectedShip == id(ship)):
                selected = item
            self.treeWidget.expandItem(parent)
            self.treeWidget.expandItem(ancestor)

        self.treeWidget.resizeColumnToContents(0)
        self.treeWidget.resizeColumnToContents(1)
        if selected is not None:
            selected.setSelected(True)
            self.treeWidget.setCurrentItem(selected)

        return


    def addShip(self):
        ship = ships.Ship(" Unknown", " Unknown", " Unknown")
        self.ships.addShip(ship)
        self.populateList()
        self.populateTree()
        self.populateTable(id(ship))
        self.tableWidget.setFocus()
        self.tableWidget.editItem(self.tableWidget.currentItem())

    def tableItemChanged(self, item):
        ship = self.currentTableShip()
        if ship is None:
            return
        column = self.tableWidget.currentColumn()
        if column == ships.NAME:
            ship.name = item.text().trimmed()
        elif column == ships.OWNER:
            ship.owner = item.text().trimmed()
        elif column == ships.COUNTRY:
            ship.country = item.text().trimmed()
        elif column == ships.DESCRIPTION:
            ship.description = item.text().trimmed()
        elif column == ships.TEU:
            ship.teu = item.text().toInt()[0]
        self.ships.dirty = True
        self.populateList()
        self.populateTree()

    def currentTableShip(self):
        item = self.tableWidget.item(self.tableWidget.currentRow(), 0)
        if item is None:
            return None
        return self.ships.ship(item.data(Qt.UserRole).toLongLong()[0])

    def removeShip(self):
        ship = self.currentTableShip()
        if ship is None:
            return
        if QMessageBox.question(self, "Ships - Remove",
                                QString("Remove %1 of %2/%3?")
                                .arg(ship.name).arg(ship.owner).arg(ship.country),
                                QMessageBox.Yes | QMessageBox.No) == QMessageBox.No:
            return
        self.ships.removeShip(ship)
        self.populateList()
        self.populateTree()
        self.populateTable()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainForm()
    form.show()
    app.exec_()

