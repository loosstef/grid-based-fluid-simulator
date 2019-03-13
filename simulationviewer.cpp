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

void SimulationViewer::updateView(QImage* image)
{
    QPixmap pixmap = QPixmap::fromImage(*image);
    this->simulationWidth = pixmap.width();
    this->simulationHeight = pixmap.height();
    this->setPixmap(pixmap.scaled(this->viewWidth, this->viewHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void SimulationViewer::mousePressEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    Coordinate simulationCoordinate = this->calculatePositionInSimulation(ev->x(), ev->y());
    if(leftClicked) {
        emit mouseLeftButtonClicked(simulationCoordinate.x, simulationCoordinate.y);
        mouseLeftBtnDown = true;
    }
    if(rightClicked) {
        emit mouseRightButtonClicked(ev->x(), ev->y());
        mouseRightBtnDown = true;
    }
}

void SimulationViewer::mouseMoveEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    Coordinate simulationCoordinate = this->calculatePositionInSimulation(ev->x(), ev->y());
    if(leftClicked) {
        emit mouseLeftButtonMoved(simulationCoordinate.x, simulationCoordinate.y);
    }
    if(rightClicked) {
        emit mouseRightButtonMoved(simulationCoordinate.x, simulationCoordinate.y);
    }
}

void SimulationViewer::mouseReleaseEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    if(!leftClicked && this->mouseLeftBtnDown) {
        emit mouseLeftButtonReleased();
        this->mouseLeftBtnDown = false;
    }
    if(!rightClicked && this->mouseRightBtnDown) {
        emit mouseRightButtonReleased();
        this->mouseRightBtnDown = false;
    }
}

/**
 * Calculate the position in the simulation, based on the position on the
 * scaled image of the simulation
 * @brief SimulationViewer::calculatePositionInSimulation
 * @param viewX x-coordinate in view
 * @param viewY y-coordinate in view
 * @return the coordinate in the simulation
 */
Coordinate SimulationViewer::calculatePositionInSimulation(int viewX, int viewY)
{
    Coordinate simCoordinate;
    simCoordinate.x = (int)(((float)viewX/this->viewWidth) * this->simulationWidth);
    simCoordinate.y = (int)((float)viewY/this->viewHeight * this->simulationHeight);
    return simCoordinate;
}
