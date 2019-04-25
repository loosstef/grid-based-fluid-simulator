#include "painthandler.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "field.h"
#include "painttool.h"
#include "painttoolcontroller.h"
#include "simulationviewer.h"

PaintHandler::PaintHandler(Field* field)
{
    mSmokePaintTool = new PaintTool(field->getSmokeDensityGrid());
    mSmokePaintToolController = mSmokePaintTool->getController();
}

void PaintHandler::connectToSimulationViewer(SimulationViewer *simViewer)
{
    connect(simViewer, &SimulationViewer::mouseLeftButtonClicked, this, &PaintHandler::leftClicked);
    connect(simViewer, &SimulationViewer::mouseLeftButtonMoved, this, &PaintHandler::leftMoved);
}

void PaintHandler::connectToSettings(QSpinBox *brushSize, QDoubleSpinBox *hardness)
{
    mSmokePaintToolController->connectToBrushSettings(brushSize, hardness);
}

void PaintHandler::leftClicked(int x, int y)
{
    mSmokePaintTool->drawPoint(x, y);
}

void PaintHandler::leftMoved(int x, int y)
{
    mSmokePaintTool->drawPoint(x, y);
}

void PaintHandler::rightClicked(int x, int y)
{

}

void PaintHandler::rightMoved(int x, int y)
{

}
