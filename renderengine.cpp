#include "renderengine.h"
#include <QtMath>

RenderEngine::RenderEngine(int width, int height, bool showVelocity) :
    mWidth(width), mHeight(height), mShowVelocity(showVelocity)
{

}

/**
 * Render an image based on a given field
 * @brief RenderEngine::render
 * @param image
 * @param field
 */
QImage* RenderEngine::render(Field *field)
{
    QImage* image = new QImage(field->getWidth(), field->getHeight(), QImage::Format_RGB32);
    Grid* smokeDensityGrid = field->getSmokeDensityGrid();
    // calculate the unscaled image
    for(int x = 0; x < field->getWidth(); ++x) {
        for(int y = 0; y < field->getHeight(); ++y) {
            float colorIntensity = this->valueToColorIntensity(smokeDensityGrid->get(x, y));
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            if(color.isValid()) {
                image->setPixelColor(x, y, color);
            } else {
                throw std::exception();
            }
        }
    }
    // scale the image
    QImage scaledImage = image->scaled(this->mWidth, this->mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage* scaledImageHeap = new QImage(scaledImage);
    delete image;
    return scaledImageHeap;
}

/**
 * Get the color-intensity belonging to the value (by example the density)
 * of the fluid in a certain point.
 * @brief RenderEngine::valueToColorIntensity
 * @param value the value of the fluid in a certain point
 * @return the color-intensity of the given value, always between 0 and 255
 */
float RenderEngine::valueToColorIntensity(float value)
{
    float colorIntensity = qSqrt(value)*75;
    if(colorIntensity < 0) {
        colorIntensity = 0;
    } else if(colorIntensity > 255) {
        colorIntensity = 255;
    }
    return colorIntensity;
}
