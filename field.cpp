#include "field.h"
#include <QImage>

#include <cstdlib>
#include <QtMath>

Field::Field(int simWidth, int simHeight) :
    simWidth(simWidth), simHeight(simHeight),
    density(simWidth, simHeight), horizontalVelocity(simWidth, simHeight), verticalVelocity(simWidth, simHeight)
{

}


/**
 * Generate an image based on this field
 * @brief Field::render
 * @param pixmap this pixmap will be filled in
 */
// TODO: clean up comments
void Field::render(QImage* image)
{
    //QImage* image = new QImage(this->simWidth, this->simHeight, QImage::Format_RGB32);
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            float colorIntensity = this->valueToColorIntensity(this->density.get(x, y));
            if(colorIntensity > 255) {
                printf("hoi");
            }
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            if(!color.isValid()) {
                printf("oei");
            }
            image->setPixelColor(x, y, color);
        }
    }
    //QPixmap generatedPixmap = QPixmap::fromImage(*image);
    //delete image;
    //pixmap = new QPixmap(generatedPixmap);
}

/**
 * Get the color-intensity belonging to the value (by example the density)
 * of the fluid in a certain point.
 * @brief Field::valueToColorIntensity
 * @param value the value of the fluid in a certain point
 * @return the color-intensity of the given value, always between 0 and 255
 */
float Field::valueToColorIntensity(float value)
{
    float colorIntensity = qSqrt(value)*75;
    if(colorIntensity < 0) {
        colorIntensity = 0;
    } else if(colorIntensity > 255) {
        colorIntensity = 255;
    }
    return colorIntensity;
}
