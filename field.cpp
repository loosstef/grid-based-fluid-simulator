#include "field.h"
#include <QImage>

#include <cstdlib>

Field::Field(int width, int height) :
    width(width), height(height),
    density(width, height), horizontalVelocity(width, height), verticalVelocity(width, height)
{

}

/**
 * Generate an image based on this field
 * @brief Field::render
 * @return a pixmap representing the rendered version of this field
 */
QPixmap *Field::render()
{
    QImage* image = new QImage(this->width, this->height, QImage::Format_RGB32);
    for(int x = 0; x < this->width; ++x) {
        for(int y = 0; y < this->height; ++y) {
            float colorIntensity = this->density.get(x, y);
            //QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            image->setPixelColor(x, y, color);
        }
    }
    QPixmap pixmap = QPixmap::fromImage(*image);
    delete image;
    return new QPixmap(pixmap);
}
