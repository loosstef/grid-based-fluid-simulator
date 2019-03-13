#include "grid.h"
#include <cstdlib>

/**
 * Constructor
 */
Grid::Grid(int width, int height): width(width), height(height)
{
    this->values = new float[width*height];
    for(int i = 0; i < width*height; ++i) {
        this->values[i] = 0;
    }
}

/**
 * Get the value of this grid on a certain location
 * @brief Grid::get
 * @param x x-coordinate
 * @param y y-coordinate
 * @return the value on location (x,y)
 */
float Grid::get(int x, int y)
{
    return this->values[x + y*this->width];
}

/**
 * Set the value of a point in this grid
 * @brief Grid::set
 * @param x x-coordinate
 * @param y y-coordinate
 * @param value the value
 * @return true if the coordinate was valid
 */
bool Grid::set(int x, int y, float value)
{
    if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
        this->values[x + y*this->width] = value;
        return true;
    } else {
        return false;
    }
}
