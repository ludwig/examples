#!/usr/bin/env python

from PyQt4.QtCore import *
from PyQt4.QtGui import *
import moviedata
import ui_addeditmoviedlg

class AddEditMovieDlg(QDialog, ui_addeditmoviedlg.Ui_AddEditMovieDlg):
    
    def __init__(self, movies, movie=None, parent=None):
        super(AddEditMovieDlg, self).__init__(parent)
        self.setupUi(self)

        self.movies = movies
        self.movie = movie
        self.acquiredDateEdit.setDisplayFormat(moviedata.DATEFORMAT)
        if movie is not None:
            self.titleLineEdit.setText(movie.title)
            self.yearSpinBox.setValue(movie.year)
            self.minutesSpinBox.setValue(movie.minutes)
            self.acquiredDateEdit.setDate(movie.acquired)
            self.notesTextEdit.setPlainText(movie.notes)
            self.buttonBox.button(QDialogButtonBox.Ok).setText("&Accept")
            self.setWindowTitle("My Movies - Edit Movie")
        else:
            today = QDate.currentDate()
            self.acquiredDateEdit.setDateRange(today.addDays(-5), today)
            self.acquiredDateEdit.setDate(today)

        self.on_titleLineEdit_textEdited(QString())

    @pyqtSignature("QString")
    def on_titleLineEdit_textEdited(self, text):
        self.buttonBox.button(QDialogButtonBox.Ok).setEnabled(
            not self.titleLineEdit.text().isEmpty())


    def accept(self):
        title = self.titleLineEdit.text()
        year = self.yearSpinBox.value()
        minutes = self.minutesSpinBox.value()
        notes = self.notesTextEdit.toPlainText()
        if self.movie is None:
            acquired = self.acquiredDateEdit.date()
            self.movie = moviedata.Movie(title, year, minutes,
                                         acquired, notes)
            self.movies.add(self.movie)
        else:
            self.movies.updateMovie(self.movie, title, year,
                                    minutes, notes)
        QDialog.accept(self)

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    form = AddEditMovieDlg(0)
    form.show()
    app.exec_()
