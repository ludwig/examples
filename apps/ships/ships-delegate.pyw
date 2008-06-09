#!/usr/bin/env python

import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import ships

MAC = "qt_mac_set_native_menubar" in dir()


class MainForm(QDialog):

    def __init__(self, parent=None):
        super(MainForm, self).__init__(parent)

        self.model = ships.ShipTableModel(QString("ships.dat"))

        tableLabel1 = QLabel("Table &1")
        self.tableView1 = QTableView()
        tableLabel1.setBuddy(self.tableView1)
        self.tableView1.setModel(self.model)
        self.tableView1.setItemDelegate(ships.ShipDelegate(self))

        addShipButton = QPushButton("&Add Ship")
        removeShipButton = QPushButton("&Remove Ship")
        quitButton = QPushButton("&Quit")
        if not MAC:
            addShipButton.setFocusPolicy(Qt.NoFocus)
            removeShipButton.setFocusPolicy(Qt.NoFocus)
            quitButton.setFocusPolicy(Qt.NoFocus)

        buttonLayout = QHBoxLayout()
        buttonLayout.addWidget(addShipButton)
        buttonLayout.addWidget(removeShipButton)
        buttonLayout.addStretch()
        buttonLayout.addWidget(quitButton)


        vbox = QVBoxLayout()
        vbox.addWidget(tableLabel1)
        vbox.addWidget(self.tableView1)
        widget = QWidget()
        widget.setLayout(vbox)


        layout = QVBoxLayout()
        layout.addWidget(widget)
        layout.addLayout(buttonLayout)

        self.setLayout(layout)

        for tableView in (self.tableView1,):
            header = tableView.horizontalHeader()
            self.connect(header, SIGNAL("sectionClicked(int)"), self.sortTable)

        self.connect(addShipButton, SIGNAL("clicked()"), self.addShip)
        self.connect(removeShipButton, SIGNAL("clicked()"), self.removeShip)
        self.connect(quitButton, SIGNAL("clicked()"), self.accept)

        self.setWindowTitle("Ships (delegate)")
        QTimer.singleShot(0, self.initialLoad)
        return

    def initialLoad(self):
        if not QFile.exists(self.model.filename):
            for ship in ships.generateFakeShips():
                self.model.ships.append(ship)
                self.model.owners.add(unicode(ship.owner))
                self.model.countries.add(unicode(ship.country))
            self.model.reset()
            self.model.dirty = False
        else:
            try:
                self.model.load()
            except IOError, e:
                QMessageBox.warning(self, "Ships - Error", "Failed to load: %s" % e)

        self.model.sortByName()
        self.resizeColumns()

    def resizeColumns(self):
        self.tableView1.resizeColumnsToContents()


    def reject(self):
        self.accept()

    def accept(self):
        if self.model.dirty and QMessageBox.question(self, "Ships - Save?",
                                                     "Save unsaved changes?",
                                                     QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes:
            try:
                self.model.save()
            except IOError, e:
                QMessageBox.warning(self, "Ships - Error", "Failed to save: %s" % e)
        QDialog.accept(self)
        return



    def sortTable(self, section):
        if section in (ships.OWNER, ships.COUNTRY):
            self.model.sortByCountryOwner()
        else:
            self.model.sortByName()
        self.resizeColumns()


    def addShip(self):
        row = self.model.rowCount()
        self.model.insertRows(row)
        index = self.model.index(row, 0)
        tableView = self.tableView1
        #if self.tableView2.hasFocus()
        #   tableView = self.tableView2
        tableView.setFocus()
        tableView.setCurrentIndex(index)
        tableView.edit(index)
        return

    def removeShip(self):
        tableView = self.tableView1
        #if self.tableView2.hasFocus()
        #   tableView = self.tableView2
        index = tableView.currentIndex()
        if not index.isValid():
            return
        row = index.row()
        name = self.model.data(self.model.index(row, ships.NAME)).toString()
        owner = self.model.data(self.model.index(row, ships.OWNER)).toString()
        country = self.model.data(self.model.index(row, ships.COUNTRY)).toString()
        if QMessageBox.question(self, "Ships - Remove",
                                QString("Remove %1 of %2/%3?")
                                .arg(name).arg(owner).arg(country),
                                QMessageBox.Yes | QMessageBox.No) == QMessageBox.No:
            return
        self.model.removeRows(row)
        self.resizeColumns()
        return


if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainForm()
    form.show()
    app.exec_()

