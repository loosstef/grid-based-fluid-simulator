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
        qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - timer;
        if(deltaTime < this->sleepPerLoop) {
            this->msleep(this->sleepPerLoop-deltaTime);
        }
        deltaTime = QDateTime::currentMSecsSinceEpoch() - timer;
        timer = QDateTime::currentMSecsSinceEpoch();
        if(this->runLock.tryLock()) {
            this->runLock.unlock();
            this->mSimField->simulateNextStep(deltaTime);
        }
        QImage* renderedImage = this->mRenderEngine->render(this->mSimField);
        emit FieldUpdated(renderedImage);
    }
}
/*
            // Sleep as debug tool
            if(deltaTime < this->sleepPerLoop) {
                this->msleep(this->sleepPerLoop-deltaTime);
            }*/
