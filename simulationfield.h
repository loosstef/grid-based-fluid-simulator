#ifndef SIMULATIONFIELD_H
#define SIMULATIONFIELD_H

#include "field.h"


class SimulationField : public Field
{
public:
    SimulationField(int width, int height);
    bool simulateNextStep(int deltaTime);
};

#endif // SIMULATIONFIELD_H
