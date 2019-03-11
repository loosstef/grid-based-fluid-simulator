#ifndef FIELD_H
#define FIELD_H

#include "grid.h"
#include <QPixmap>

class Field
{
public:
    Field(int width, int height);
    QPixmap *render();
protected:
    int width;
    int height;
    Grid density;
    Grid horizontalVelocity;
    Grid verticalVelocity;
};

#endif // FIELD_H
