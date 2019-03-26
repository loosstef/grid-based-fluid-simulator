#include "painttoolcontroller.h"

#include "painttool.h"

PaintToolController::PaintToolController(PaintTool* paintTool, QObject* parent) :
    QObject(parent), mPaintTool(paintTool)
{

}

void PaintToolController::connectToBrushSettings(QSpinBox *brushSizeField, QDoubleSpinBox* brushHardnessField)
{
    connect(brushSizeField, qOverload<int>(&QSpinBox::valueChanged), this, &PaintToolController::brushSizeChanged);
    connect(brushHardnessField, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &PaintToolController::brushHardnessChanged);
}

void PaintToolController::brushSizeChanged(int i)
{
    this->mPaintTool->setSize(i);
}

void PaintToolController::brushHardnessChanged(double i)
{
    this->mPaintTool->setHardness(i);
}
