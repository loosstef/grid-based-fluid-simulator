#include "renderengine.h"
#include <QtMath>
#include <QPointF>
#include "grid.h"
#include "field.h"
#include "renderenginecontroller.h"

const Qt::GlobalColor VELOCITY_COLOR = Qt::red;
const int VEL_VECTOR_SPARSENESS = 3;

RenderEngine::RenderEngine(const int width,  const int height, const bool showVelocity) :
    mWidth(width), mHeight(height), mShowVelocity(showVelocity)
{

}

/**
 * Render an image based on a given field
 * @brief RenderEngine::render
 * @param field
 * @return the rendered image
 */
QImage* RenderEngine::render(const Field *field)
{
    QImage* image = new QImage(field->getWidth(), field->getHeight(), QImage::Format_RGB32);
    // calculate the unscaled image
    if(this->mRenderType == smoke) {
        renderSmoke(image, field);
    } else if(this->mRenderType == density) {
        renderDensity(image, field);
    }
    // scale the image
    QImage scaledImage = image->scaled(this->mWidth, this->mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage* scaledImageHeap = new QImage(scaledImage);
    delete image;

    if(this->mShowVelocity) {
        this->renderVelocity(scaledImageHeap, field);
    }

    return scaledImageHeap;
}

RenderEngineController *RenderEngine::getController()
{
    return new RenderEngineController(this);
}

void RenderEngine::setVelocityScale(const float velScale)
{
    this->mVelocityScale = velScale;
}

/**
 * Toggle the visibility of the velocity-vectors
 * @brief RenderEngine::toggleShowVelocity
 * @param visible true if you want the vectors to be visible
 */
void RenderEngine::toggleShowVelocity(const bool visible)
{
    this->mShowVelocity = visible;
}

/**
 * Render an image based on the smoke-density in the field
 * @brief RenderEngine::renderSmoke
 * @param image the image to override
 * @param field the field containing the smoke to render
 */
void RenderEngine::renderSmoke(QImage *image, const Field* field) const
{
    Grid* smokeDensityGrid = field->getSmokeDensityGrid();
    for(int x = 0; x < field->getWidth(); ++x) {
        for(int y = 0; y < field->getHeight(); ++y) {
            float colorIntensity = this->valueToColorIntensity(smokeDensityGrid->get(x, y), RenderEngine::smokeToColorIntensity);
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            if(color.isValid()) {
                image->setPixelColor(x, y, color);
            } else {
                image->setPixelColor(x, y, QColor(0, 255, 0));
            }
        }
    }
}

void RenderEngine::renderDensity(QImage *image, const Field *field) const
{
    Grid* densityGrid = field->getDensityGrid();
    for(int x = 0; x < field->getWidth(); ++x) {
        for(int y = 0; y < field->getHeight(); ++y) {
            float colorIntensity = this->valueToColorIntensity(densityGrid->get(x, y), RenderEngine::densityToColorIntensity);
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            if(color.isValid()) {
                image->setPixelColor(x, y, color);
            } else {
                image->setPixelColor(x, y, QColor(0, 255, 0));
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
void RenderEngine::renderVelocity(QImage *image, const Field *field)
{
    Grid* horVel = field->getHorizontalVelocity();
    Grid* verVel = field->getVerticalVelocity();

    int pointsPairsIndex = 0;
    QPoint* pointsPairs = new QPoint[field->getDensityGrid()->getSize()*2];

    float pixWidth = (float)image->width() / (float)field->getWidth();
    float pixHeight = (float)image->height() / (float)field->getHeight();
    int offSetX = (int)(pixWidth/2);
    int offSetY = (int)(pixHeight/2);

    for(int x = 0; x < field->getWidth(); x += VEL_VECTOR_SPARSENESS) {
        for (int y = 0; y < field->getHeight(); y += VEL_VECTOR_SPARSENESS) {
            int baseX = (int)x*(pixWidth) + offSetX;
            int baseY = (int)y*(pixHeight) + offSetY;
            int endX = baseX+horVel->get(x, y)*this->mVelocityScale;
            int endY = baseY+verVel->get(x, y)*this->mVelocityScale;
            pointsPairs[pointsPairsIndex].setX(baseX);
            pointsPairs[pointsPairsIndex].setY(baseY);
            ++pointsPairsIndex;
            pointsPairs[pointsPairsIndex].setX(endX);
            pointsPairs[pointsPairsIndex].setY(endY);
            ++pointsPairsIndex;
        }
    }
    this->mPainter.begin(image);
    mPainter.setPen(VELOCITY_COLOR);
    this->mPainter.drawLines(pointsPairs, pointsPairsIndex/2);
    this->mPainter.end();
    delete pointsPairs;
}

/**
 * Get the color-intensity belonging to the value (by example the density)
 * of the fluid in a certain point.
 * @brief RenderEngine::valueToColorIntensity
 * @param value the value of the fluid in a certain point
 * @return the color-intensity of the given value, always between 0 and 255
 */
float RenderEngine::valueToColorIntensity(float value, float (*f)(float)) const
{
    if(value < 0) {
        value = 0;
    }
    float colorIntensity = f(value);
    if(colorIntensity < 0) {
        colorIntensity = 0;
    } else if(colorIntensity > 255) {
        colorIntensity = 255;
    }
    return colorIntensity;
}

float RenderEngine::smokeToColorIntensity(float value)
{
    return qSqrt(value) * 75;
}

float RenderEngine::densityToColorIntensity(float value)
{
    return value * 65;
}
