#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:

    Window();

private:

    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
};

#endif
