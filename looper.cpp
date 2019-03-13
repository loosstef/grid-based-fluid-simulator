#include "looper.h"
#include <QDateTime>


/**
 * Constructor
 * @brief Looper::Looper
 * @param simField the simulation-field in wich the simulation will happen
 * @param parent
 */
Looper::Looper(SimulationField *simField, QObject *parent) :
    mSimField(simField), QThread(parent)
{

}

void Looper::run() {
    // init data
    qint64 timeOfLastUpdate = QDateTime::currentMSecsSinceEpoch();
    QImage imageBuffer = QImage(this->mSimField->getWidth(), this->mSimField->getHeight(), QImage::Format_RGB32);

    // start loop
    while(true) {
        this->mSimField->simulateNextStep(100);
        this->mSimField->render(&imageBuffer);
        qint64 timeSinceLastUpdate = QDateTime::currentMSecsSinceEpoch() - timeOfLastUpdate;
        if(timeSinceLastUpdate < 10) {
            this->usleep(1000 * (10 - timeSinceLastUpdate));
        }
        emit FieldUpdated(&imageBuffer);
        timeOfLastUpdate = QDateTime::currentMSecsSinceEpoch();
    }
}
