#ifndef GRID_H
#define GRID_H

#include<QImage>

class Grid
{
public:
    Grid(int width, int height);
    float get(int x, int y);
    bool set(int x, int y, float value);
private:
    int width;
    int height;
    float* values;
};

#endif // GRID_H
