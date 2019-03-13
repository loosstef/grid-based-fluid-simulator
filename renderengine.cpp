#include "renderengine.h"
#include <QtMath>
#include <QPainter>

const Qt::GlobalColor VELOCITY_COLOR = Qt::red;
const int VEL_VECTOR_SPARSENESS = 5;
const int VEL_SCALE = 1;

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
    // calculate the unscaled image
    renderSmoke(image, field);
    // scale the image
    QImage scaledImage = image->scaled(this->mWidth, this->mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage* scaledImageHeap = new QImage(scaledImage);
    delete image;

    if(this->mShowVelocity) {
        this->renderVelocity(scaledImageHeap, field);
    }

    return scaledImageHeap;
}

void RenderEngine::toggleShowVelocity()
{
    if(this->mShowVelocity) {
        this->mShowVelocity = false;
    } else {
        this->mShowVelocity = true;
    }
}

/**
 * Render an image based on the smoke-density in the field
 * @brief RenderEngine::renderSmoke
 * @param image the image to override
 * @param field the field containing the smoke to render
 */
void RenderEngine::renderSmoke(QImage *image, Field* field)
{
    Grid* smokeDensityGrid = field->getSmokeDensityGrid();
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
}

/**
 * Override an image based on the velocities in a given field
 * @brief RenderEngine::renderVelocity
 * @param image the image to override
 * @param field the field containing the velocity to render
 */
void RenderEngine::renderVelocity(QImage *image, Field *field)
{
    QPainter* painter = new QPainter(image);
    painter->setPen(VELOCITY_COLOR);

    Grid* horVel = field->getHorizontalVelocity();
    Grid* verVel = field->getVerticalVelocity();

    for(int x = 0; x < field->getWidth(); x += VEL_VECTOR_SPARSENESS) {
        for (int y = 0; y < field->getHeight(); y += VEL_VECTOR_SPARSENESS) {
            int baseX = x*((int)(image->width()/field->getWidth()));
            int baseY = y*((int)(image->height()/field->getHeight()));
            int endX = baseX+horVel->get(x, y)*VEL_SCALE;
            int endY = baseY+verVel->get(x, y)*VEL_SCALE;
            painter->drawLine(baseX, baseY, endX, endY);
        }
    }

    painter->end();
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
