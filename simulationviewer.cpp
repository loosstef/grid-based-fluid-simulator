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

void SimulationViewer::setSize(int simWidth, int simHeight, int viewWidth, int viewHeight)
{
    this->mSimulationWidth = simWidth;
    this->mSimulationHeight = simHeight;
    this->mViewWidth = viewWidth;
    this->mViewHeight = viewHeight;
}

/**
 * @brief SimulationViewer::updateView
 * @param image
 * @post image is deleted
 */
void SimulationViewer::updateView(QImage* image)
{
    QPixmap pixmap = QPixmap::fromImage(*image);
    this->setPixmap(pixmap);
}

void SimulationViewer::mousePressEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    Vector2D simulationCoordinate = this->calculatePositionInSimulation(ev->x(), ev->y());
    if(leftClicked) {
        emit mouseLeftButtonClicked(simulationCoordinate.x, simulationCoordinate.y);
        mouseLeftBtnDown = true;
    }
    if(rightClicked) {
        emit mouseRightButtonClicked(simulationCoordinate.x, simulationCoordinate.y, ev->x(), ev->y());
        mouseRightBtnDown = true;
    }
}

void SimulationViewer::mouseMoveEvent(QMouseEvent *ev)
{
    bool leftClicked = (ev->buttons() & Qt::LeftButton) != 0;
    bool rightClicked = (ev->buttons() & Qt::RightButton) != 0;
    Vector2D simulationCoordinate = this->calculatePositionInSimulation(ev->x(), ev->y());
    if(leftClicked) {
        emit mouseLeftButtonMoved(simulationCoordinate.x, simulationCoordinate.y);
    }
    if(rightClicked) {
        emit mouseRightButtonMoved(simulationCoordinate.x, simulationCoordinate.y, ev->x(), ev->y());
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
Vector2D SimulationViewer::calculatePositionInSimulation(int viewX, int viewY)
{
    Vector2D simCoordinate;
    simCoordinate.x = (int)(((double)viewX/this->mViewWidth) * this->mSimulationWidth);
    simCoordinate.y = (int)((double)viewY/this->mViewHeight * this->mSimulationHeight);
    return simCoordinate;
}
