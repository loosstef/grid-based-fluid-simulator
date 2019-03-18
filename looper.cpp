#include "looper.h"
#include <QDateTime>
#include "renderengine.h"

const int MAX_FPS_COUNT = 500;


/**
 * Constructor
 * @brief Looper::Looper
 * @param simField the simulation-field in wich the simulation will happen
 * @param parent
 */
Looper::Looper(SimulationField *simField, int width, int height, QObject *parent) :
    mSimField(simField), QThread(parent), mWidth(width), mHeight(height)
{

}

void Looper::run() {
    // init data
    RenderEngine renderEngine = RenderEngine(mWidth, mHeight, true);
    qint64 timer = QDateTime::currentMSecsSinceEpoch();

    // start loop
    while(true) {
        qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - timer;
        this->mSimField->simulateNextStep(deltaTime);
        timer = QDateTime::currentMSecsSinceEpoch();
        QImage* renderedImage = renderEngine.render(this->mSimField);
        emit FieldUpdated(renderedImage);
    }
}
