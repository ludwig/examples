// ---------------------------------------
// simpleplot.cpp
//
// A simple example which shows how to use SurfacePlot
//----------------------------------------

#include <cmath>
#include <qapplication.h>
#include <qwt3d_surfaceplot.h>
#include <qwt3d_function.h>

using namespace Qwt3D;

class Rosenbrock : public Function
{
public:
    Rosenbrock(SurfacePlot* pw) : Function(pw)
    {
    }

    double operator()(double x, double y)
    {
        return log((1-x)*(1-x) + 100*(y-x*x)*(y-x*x)) / 8;
    }
};

class Plot : public SurfacePlot
{
public:
    Plot();
};


Plot::Plot()
{
    setTitle("A Simple SurfacePlot Demonstration");

    Rosenbrock rosenbrock(this);

    rosenbrock.setMesh(41, 31);
    rosenbrock.setDomain(-1.73, 1.5, -1.5, 1.5);
    rosenbrock.setMinZ(-10);

    rosenbrock.create();

    setRotation(30, 0, 15);
    setScale(1, 1, 1);
    setShift(0.15, 0, 0);
    setZoom(0.9);

    for (unsigned i = 0; i != coordinates()->axes.size(); ++i)
    {
        coordinates()->axes[i].setMajors(7);
        coordinates()->axes[i].setMinors(4);
    }

    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    coordinates()->axes[Z1].setLabelString(QChar(0x38f)); // // Omega - see http://www.unicode.org/charts/

    setCoordinateStyle(BOX);

    updateData();
    updateGL();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plot plot;
    plot.resize(800, 600);
    plot.show();
    return a.exec();
}
