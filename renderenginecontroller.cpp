#include "renderenginecontroller.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
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
