#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include "vector2D.h"

class QWidget;

class SimulationViewer : public QLabel
{
    Q_OBJECT
public:
    SimulationViewer(QWidget* parent);
    void setSize(int simWidth, int simHeight, int viewWidth, int viewHeight);
signals:
    void mouseLeftButtonClicked(int x, int y);
    void mouseLeftButtonMoved(int x, int y);
    void mouseLeftButtonReleased();
    void mouseRightButtonClicked(int x, int y, int viewX, int viewY);
    void mouseRightButtonMoved(int x, int y, int viewX, int viewY);
    void mouseRightButtonReleased();
public slots:
    void updateView(QImage* image);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    int mSimulationWidth = 1;
    int mSimulationHeight = 1;
    int mViewWidth = 1;
    int mViewHeight = 1;
    bool mouseLeftBtnDown = false;
    bool mouseRightBtnDown = false;
    Vector2D calculatePositionInSimulation(int viewX, int viewY);
};

#endif // SIMULATIONVIEWER_H
