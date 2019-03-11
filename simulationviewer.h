#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>

class QWidget;

class SimulationViewer : public QLabel
{
    Q_OBJECT
public:
    SimulationViewer(QWidget* parent);
    void setSize(int x, int y);
signals:
    void mouseLeftButtonDown(int x, int y);
    void mouseRightButtonDown(int x, int y);
public slots:
    void updateView(QPixmap* pixmap);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
private:
    int viewWidth = 400;
    int viewHeight = 400;
    int simulationWidth = 1;
    int simulationHeight = 1;
    void mouseEvent(QMouseEvent *ev);
};

#endif // SIMULATIONVIEWER_H
