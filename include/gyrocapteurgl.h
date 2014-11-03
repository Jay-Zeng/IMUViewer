 
#ifndef GYROGLWIDGET_H
#define GYROGLWIDGET_H

#include "capteurgl.h"
#include <QGLWidget>
#include <QTimerEvent>

class GyroCapteurGL : public QGLWidget
{
    Q_OBJECT
    public:
         explicit GyroCapteurGL(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent( QKeyEvent *keyEvent );
    void timerEvent(QTimerEvent);

    void setCentrale(Centrale *);

public slots:
    void updateGL();

private:
    Centrale* _pIMU;

};

#endif
