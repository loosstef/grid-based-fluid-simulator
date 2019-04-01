#ifndef GRID_H
#define GRID_H

#include<QImage>
#include<QMutex>

class Grid
{
public:
    Grid(int width, int height, float initValue = 0);
    Grid(float* values, int width, int height);
    Grid(Grid* grid);
    ~Grid();
    int getWidth() {return this->width;}
    int getHeight() {return this->height;}
    float get(int x, int y);
    float get(int i) {return this->values[i];}
    float *getRaw() {return this->values;}
    int getSize() {return width*height;}
    bool set(int x, int y, float value);
    void set(float* values);
    bool add(int x, int y, float value);
    void reset(float val);
private:
    int width;
    int height;
    float* values;
};

#endif // GRID_H
