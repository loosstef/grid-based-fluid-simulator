#include "grid.h"
#include <cstdlib>

#include <math.h>

/**
 * Constructor
 */
Grid::Grid(int width, int height, float initValue):
    width(width), height(height)
{
    this->values = new float[width*height];
    this->reset(initValue);
}

Grid::Grid(Grid *grid)
{
    this->values = new float[grid->getWidth()*grid->getHeight()];
    for(int i = 0; i < grid->getWidth()*grid->getHeight(); ++i) {
        this->values[i] = grid->get(i);
    }
    this->width = grid->getWidth();
    this->height = grid->getHeight();
}

Grid::~Grid()
{
    delete this->values;
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
    // real code
    if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
        this->values[x + y*this->width] = value;
        return true;
    } else {
        return false;
    }
}

/**
 * Add a value to the value of a point in this grid
 * @brief Grid::add
 * @param x x-coordinate
 * @param y y-coordinate
 * @param value the value to add to this cell
 * @return true if the given coordinate was valid
 */
bool Grid::add(int x, int y, float value)
{
    if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
        this->values[x + y*this->width] += value;
        return true;
    } else {
        return false;
    }
}

void Grid::reset(float val)
{
    for(int i = 0; i < this->width * this->height; ++i) {
        this->values[i] = val;
    }
}
