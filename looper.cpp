#include "looper.h"
#include <QDateTime>
#include "renderengine.h"
#include "simulationfield.h"

const int MAX_FPS_COUNT = 500;


/**
 * Constructor
 * @brief Looper::Looper
 * @param simField the simulation-field in wich the simulation will happen
 * @param parent
 */
Looper::Looper(SimulationField* simField, RenderEngine* renderEngine, QObject* parent) :
    mSimField(simField), mRenderEngine(renderEngine), QThread(parent)
{

}

void Looper::run() {
    // init data
    qint64 timer = QDateTime::currentMSecsSinceEpoch();

    // start loop
    while(true) {
        qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - timer;
        this->mSimField->simulateNextStep(deltaTime);
        timer = QDateTime::currentMSecsSinceEpoch();
        QImage* renderedImage = this->mRenderEngine->render(this->mSimField);
        emit FieldUpdated(renderedImage);
    }
}
