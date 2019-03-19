#include "renderenginecontroller.h"

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
void RenderEngineController::connectView(QCheckBox *toggleVelocityCheckBox)
{
    connect(toggleVelocityCheckBox, &QCheckBox::stateChanged, this, &RenderEngineController::toggleVelocityVisibility);
}

void RenderEngineController::toggleVelocityVisibility(int state)
{
    if(state == Qt::Unchecked) {
        this->mRenderEngine->toggleShowVelocity(false);
    } else {
        this->mRenderEngine->toggleShowVelocity(true);
    }
}
