#include "renderenginecontroller.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "renderengine.h"

RenderEngineController::RenderEngineController(RenderEngine* renderEngine) :
    mRenderEngine(renderEngine)
{

}

/**
 * Connect everything between the gui and the render-engine
 * @brief RenderEngineController::connect
 * @param toggleVelocityCheckBox
 */
void RenderEngineController::connectView(QCheckBox *toggleVelocityCheckBox, QDoubleSpinBox* velocityScale)
{
    connect(toggleVelocityCheckBox, &QCheckBox::stateChanged, this, &RenderEngineController::toggleVelocityVisibility);
    connect(velocityScale, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &RenderEngineController::changeVelocityScale);
}

void RenderEngineController::connectRenderTypeSelector(QComboBox *renderTypeSelector)
{
    connect(renderTypeSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &RenderEngineController::renderTypeChanged);
}

void RenderEngineController::toggleVelocityVisibility(int state)
{
    if(state == Qt::Unchecked) {
        this->mRenderEngine->toggleShowVelocity(false);
    } else {
        this->mRenderEngine->toggleShowVelocity(true);
    }
}

void RenderEngineController::changeVelocityScale(double scale)
{
    this->mRenderEngine->setVelocityScale(scale);
}

void RenderEngineController::renderTypeChanged(int newRenderTypeIndex)
{
    RenderType renderType = (RenderType) newRenderTypeIndex;
    this->mRenderEngine->setRenderType(renderType);
}
