#include "renderengine.h"
#include <QtMath>
#include <QPointF>
#include "grid.h"
#include "field.h"
#include "simulationfield.h"
#include "renderenginecontroller.h"

const Qt::GlobalColor VELOCITY_COLOR = Qt::red;

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
        renderGrid(image, field->getEnergyGrid(), RenderEngine::densityToColorIntensity);
    } else if(this->mRenderType == temperature) {
        Grid* tempGrid = this->generateTemperatureGrid(field->getMassGrid(), field->getEnergyGrid());
        renderGrid(image, tempGrid, RenderEngine::temperatureToColorIntensity);
        delete tempGrid;
    }
    // scale the image
    QImage scaledImage = image->scaled(this->mWidth, this->mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage* scaledImageHeap = new QImage(scaledImage);
    delete image;

    // draw walls
    const double widthScale = (double)this->mWidth/(double)field->getWidth();
    const double heightScale = (double)this->mHeight/(double)field->getHeight();
    renderWalls(scaledImageHeap, field->getWallsGrid(), field->getWidth(), field->getHeight(), widthScale, heightScale);

    if(this->mShowVelocity) {
        this->renderVelocity(scaledImageHeap, field);
    }

    return scaledImageHeap;
}

RenderEngineController *RenderEngine::getController()
{
    return new RenderEngineController(this);
}

void RenderEngine::setVelocityScale(const double velScale)
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
void RenderEngine::renderGrid(QImage *image, const Grid *grid, double (*f)(double)) const
{
    for(int x = 0; x < grid->getWidth(); ++x) {
        for(int y = 0; y < grid->getHeight(); ++y) {
            double colorIntensity = this->valueToColorIntensity(grid->get(x, y), f);
            QColor color = QColor(colorIntensity, colorIntensity, colorIntensity);
            Q_ASSERT(color.isValid());
            image->setPixelColor(x, y, color);
        }
    }
}

void RenderEngine::renderWalls(QImage *image, Grid *walls, int width, int height, double xScale, double yScale)
{
    this->mPainter.begin(image);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            double wallSolidity = walls->get(x, y);
            if(wallSolidity > 0) {
                double redIntensity = weightedAverage(255, 0, wallSolidity, 0, 5);
                double greenIntensity = weightedAverage(0, 255, wallSolidity, 0, 60);
                double blueIntensity = weightedAverage(255, 0, wallSolidity, 0, 60);
                QColor wallColor(redIntensity, greenIntensity, blueIntensity);
                mPainter.setPen(wallColor);
                this->mPainter.fillRect(ceil(x*xScale), ceil(y*yScale), ceil(xScale), ceil(yScale), wallColor);
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
    QPoint* pointsPairs = new QPoint[field->getMassGrid()->getSize()*2];

    double pixWidth = (double)image->width() / (double)field->getWidth();
    double pixHeight = (double)image->height() / (double)field->getHeight();
    int offSetX = (int)(pixWidth/2);
    int offSetY = (int)(pixHeight/2);

    for(int x = 0; x < field->getWidth(); x += this->mVelocitySparseness) {
        for (int y = 0; y < field->getHeight(); y += this->mVelocitySparseness) {
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
    delete[] pointsPairs;
}

/**
 * Get the color-intensity belonging to the value (by example the density)
 * of the fluid in a certain point.
 * @brief RenderEngine::valueToColorIntensity
 * @param value the value of the fluid in a certain point
 * @return the color-intensity of the given value, always between 0 and 255
 */
double RenderEngine::valueToColorIntensity(double value, double (*f)(double)) const
{
    if(value < 0) {
        value = 0;
    }
    double colorIntensity = f(value);
    if(colorIntensity < 0) {
        colorIntensity = 0;
    } else if(colorIntensity > 255) {
        colorIntensity = 255;
    }
    return colorIntensity;
}

double RenderEngine::smokeToColorIntensity(double value)
{
    return qSqrt(value) * 75;
}

double RenderEngine::densityToColorIntensity(double value)
{
    return value * 65 / 288;
}

double RenderEngine::temperatureToColorIntensity(double value)
{
    return value / 2;
}

Grid *RenderEngine::generateTemperatureGrid(Grid *mass, Grid *energy)
{
    Grid* temperatureGrid = new Grid(mass->getWidth(), mass->getHeight());
    for(int x = 0; x < mass->getWidth(); ++x) {
        for(int y = 0; y < mass->getHeight(); ++y) {
            temperatureGrid->set(x, y, (energy->get(x, y) / mass->get(x, y)));
        }
    }
    return temperatureGrid;
}

double RenderEngine::weightedAverage(double value1, double value2, double weight, double minWeight, double maxWeight)
{
    if(weight <= minWeight) {
        return value1;
    }
    if(weight >= maxWeight) {
        return value2;
    }
    double percentage = (weight - minWeight) / (maxWeight - minWeight);
    return value1 * (1-percentage) + value2 * percentage;
}
