#include "grid.h"
#include <cstdlib>

/**
 * Constructor
 */
Grid::Grid(int width, int height): width(width), height(height)
{
    this->values = new float[width*height];
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
 */
void Grid::set(int x, int y, float value)
{
    this->values[x + y*this->width] = value;
}

/**
 * Generate an image based on this grid
 * @brief Grid::render
 * @return
 */
QImage *Grid::render()
{
    QImage* image = new QImage(this->width, this->height, QImage::Format_RGB32);
    for(int x = 0; x < this->width; ++x) {
        for(int y = 0; y < this->height; ++y) {
            float colorIntensity = this->values[x+y*this->height];
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            image->setPixelColor(x, y, color);
        }
    }
    return image;
}
