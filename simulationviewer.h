#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include "coordinate.h"

class QWidget;

class SimulationViewer : public QLabel
{
    Q_OBJECT
public:
    SimulationViewer(QWidget* parent);
    void setSize(int x, int y);
signals:
    void mouseLeftButtonClicked(int x, int y);
    void mouseLeftButtonMoved(int x, int y);
    void mouseLeftButtonReleased();
    void mouseRightButtonClicked(int x, int y);
    void mouseRightButtonMoved(int x, int y);
    void mouseRightButtonReleased();
public slots:
    void updateView(QImage* image);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    int viewWidth = 400;
    int viewHeight = 400;
    int simulationWidth = 1;
    int simulationHeight = 1;
    bool mouseLeftBtnDown = false;
    bool mouseRightBtnDown = false;
    Coordinate calculatePositionInSimulation(int viewX, int viewY);
};

#endif // SIMULATIONVIEWER_H
