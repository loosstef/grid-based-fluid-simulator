#ifndef LOOPER_H
#define LOOPER_H
#include <QThread>
#include "simulationfield.h"
#include "field.h"
#include <QPixmap>

const int FIELD_WIDTH = 400;
const int FIELD_HEIGHT = 400;


class Looper : public QThread
{
    Q_OBJECT
    void run() override {
        SimulationField simField(FIELD_WIDTH, FIELD_HEIGHT);
        while(true) {
            simField.simulateNextStep(100);
            QPixmap* renderedImage = simField.render();
            emit FieldUpdated(renderedImage);
        }
    }
signals:
    void FieldUpdated(QPixmap* pixmap);
};

#endif // LOOPER_H
