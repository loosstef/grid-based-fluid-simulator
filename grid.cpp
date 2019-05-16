#include "grid.h"

#include <cstring>

/**
 * Constructor
 */
Grid::Grid(const int width, const int height, const double initValue):
    width(width), height(height)
{
    this->values = new double[width*height];
    this->reset(initValue);
}

Grid::Grid(double *values, const int width, const int height) :
    values(values), width(width), height(height)
{

}

Grid::~Grid()
{
    delete[] this->values;
}

/**
 * Get the value of this grid on a certain location
 * @brief Grid::get
 * @param x x-coordinate
 * @param y y-coordinate
 * @return the value on location (x,y)
 */
double Grid::get(int x, int y) const
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
bool Grid::set(const int x, const int y, const double value)
{
    // real code
    if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
        this->values[x + y*this->width] = value;
        return true;
    } else {
        return false;
    }
}

void Grid::set(double *values)
{
    this->values = values;
}

/**
 * Add a value to the value of a point in this grid
 * @brief Grid::add
 * @param x x-coordinate
 * @param y y-coordinate
 * @param value the value to add to this cell
 * @return true if the given coordinate was valid
 */
bool Grid::add(const int x, const int y, const double value)
{
    if(x >= 0 && y >= 0 && x < this->width && y < this->height) {
        this->values[x + y*this->width] += value;
        return true;
    } else {
        return false;
    }
}

void Grid::reset(const double val)
{
    for(int i = 0; i < this->width * this->height; ++i) {
        this->values[i] = val;
    }
}

/**
 * Make a deepcopy of this grid
 * @brief Grid::copy
 * @param grid
 * @return
 */
Grid *Grid::deepCopy(const Grid *grid)
{
    double* copyOfValues = new double[grid->getSize()];
    memcpy(copyOfValues, grid->getRaw(), sizeof(double) * grid->getSize());
    return new Grid(copyOfValues, grid->getWidth(), grid->getHeight());
}
