#ifndef GRID_H
#define GRID_H

#include<QImage>
#include<QMutex>

class Grid
{
public:
    Grid(int width, int height, float initValue = 0);
    Grid(Grid* grid);
    ~Grid();
    int getWidth() {return this->width;}
    int getHeight() {return this->height;}
    float get(int x, int y);
    float get(int i) {return this->values[i];}
    bool set(int x, int y, float value);
    bool add(int x, int y, float value);
    void reset(float val);
private:
    int width;
    int height;
    float* values;
};

#endif // GRID_H
