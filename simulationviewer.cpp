#include "simulationviewer.h"

/**
 * Update the view
 * @brief SimulationViewer::updateView
 * @param pixmap
 */
SimulationViewer::SimulationViewer(QWidget *parent) :
    QLabel(parent)
{

}

void SimulationViewer::setSize(int x, int y)
{
    this->viewWidth = x;
    this->viewHeight = y;
}

void SimulationViewer::updateView(QPixmap *pixmap)
{
    this->simulationWidth = pixmap->width();
    this->simulationHeight = pixmap->height();
    this->setPixmap(pixmap->scaled(this->viewWidth, this->viewHeight, Qt::KeepAspectRatio));
}

void SimulationViewer::mousePressEvent(QMouseEvent *ev)
{
    this->mouseEvent(ev);
}

void SimulationViewer::mouseMoveEvent(QMouseEvent *ev)
{
    this->mouseEvent(ev);
}

void SimulationViewer::mouseEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    int viewX = ev->x();
    int viewY = ev->y();
    int simulationX = (int)(((float)viewX/this->viewWidth) * this->simulationWidth);
    int simulationY = (int)((float)viewY/this->viewHeight * this->simulationHeight);
    if(leftClicked) {
        emit mouseLeftButtonDown(simulationX, simulationY);
    }
    if(rightClicked) {
        emit mouseRightButtonDown(simulationX, simulationY);
    }
}
