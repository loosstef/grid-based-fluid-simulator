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

void Looper::toggleRunning()
{
    if(this->running) {
        this->runLock.lock();
        this->running = false;
    } else {
        this->runLock.unlock();
        this->running = true;
    }
}

void Looper::run() {
    // init data
    qint64 timer = QDateTime::currentMSecsSinceEpoch();

    // start loop
    while(true) {
        if(this->runLock.tryLock()) {
            this->runLock.unlock();
            qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - timer;
            this->mSimField->simulateNextStep(deltaTime);
        }
        timer = QDateTime::currentMSecsSinceEpoch();
        QImage* renderedImage = this->mRenderEngine->render(this->mSimField);
        emit FieldUpdated(renderedImage);
        // sleep (debug tool)
        this->msleep(this->sleepPerLoop);
    }
}
