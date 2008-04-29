#include <qapplication.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = NULL) : QMainWindow(parent)
    {
        Plot *plot = new Plot(this);
        setCentralWidget(plot);

        QToolBar *toolBar = new QToolBar(this);
        QToolButton *btnLoad = new QToolButton(toolBar);
        btnLoad->setText("Load SVG");
        btnLoad->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBar->addWidget(btnLoad);
        addToolBar(toolBar);

        connect(btnLoad, SIGNAL(clicked()), plot, SLOT(loadSVG()));
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.resize(600, 400);
    w.show();

    int rv = a.exec();
    return rv;
}
