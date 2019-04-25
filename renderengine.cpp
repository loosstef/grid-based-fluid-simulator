#include "renderengine.h"
#include <QtMath>
#include <QPointF>
#include "grid.h"
#include "field.h"
#include "renderenginecontroller.h"

const Qt::GlobalColor VELOCITY_COLOR = Qt::red;
const Qt::GlobalColor WALL_COLOR = Qt::blue;
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
        renderGrid(image, field->getSmokeDensityGrid(), RenderEngine::smokeToColorIntensity);
    } else if(this->mRenderType == density) {
        renderGrid(image, field->getDensityGrid(), RenderEngine::densityToColorIntensity);
    }
    // scale the image
    QImage scaledImage = image->scaled(this->mWidth, this->mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage* scaledImageHeap = new QImage(scaledImage);
    delete image;

    // draw walls
    const float widthScale = (float)this->mWidth/(float)field->getWidth();
    const float heightScale = (float)this->mHeight/(float)field->getHeight();
    renderWalls(scaledImageHeap, field->getWalls(), field->getWidth(), field->getHeight(), widthScale, heightScale);

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
 * Render an image based on a given grid, and a translation function
 * @brief RenderEngine::renderGrid
 * @param image the image to write to
 * @param grid the grid with the nescesary info
 * @param f the translation function from value to color-intensity
 */
void RenderEngine::renderGrid(QImage *image, const Grid *grid, float (*f)(float)) const
{
    for(int x = 0; x < grid->getWidth(); ++x) {
        for(int y = 0; y < grid->getHeight(); ++y) {
            float colorIntensity = this->valueToColorIntensity(grid->get(x, y), f);
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            Q_ASSERT(color.isValid());
            image->setPixelColor(x, y, color);
        }
    }
}

void RenderEngine::renderWalls(QImage *image, const bool *walls, int width, int height, float xScale, float yScale)
{
    this->mPainter.begin(image);
    mPainter.setPen(WALL_COLOR);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            if(walls[x + width*y]) {
                this->mPainter.fillRect(ceil(x*xScale), ceil(y*yScale), ceil(xScale), ceil(yScale), WALL_COLOR);
            }
        }
    }
    this->mPainter.end();
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
