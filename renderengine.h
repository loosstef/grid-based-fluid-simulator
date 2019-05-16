#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QPainter>

class QImage;
class Field;
class Grid;
class RenderEngineController;

enum RenderType {smoke, density, temperature};

class RenderEngine : public QObject
{
    Q_OBJECT
public:
    RenderEngine(const int width, const int height, const bool showVelocity = false);
    QImage* render(const Field* field);
    void showVelocity(const bool showVelocity) {this->mShowVelocity = showVelocity;}
    RenderEngineController *getController();
    void setVelocityScale(const double velScale);
    void setVelocitySparseness(const int velSparseness) {this->mVelocitySparseness = velSparseness;}
private:
    void renderGrid(QImage* image, const Grid* grid, double (*f)(double)) const;
    void renderWalls(QImage* image, Grid* walls, int width, int height, double xScale, double yScale);
    void renderVelocity(QImage* image, const Field* field);
    double valueToColorIntensity(double value, double (*f)(double)) const;
    static double smokeToColorIntensity(double value);
    static double densityToColorIntensity(double value);
    static double temperatureToColorIntensity(double value);
    static Grid *generateTemperatureGrid(Grid* mass, Grid* energy);
    static double weightedAverage(double value1, double value2, double weight, double minWeight, double maxWeight);
    int mWidth;
    int mHeight;
    bool mShowVelocity;
    double mVelocityScale = 1;
    int mVelocitySparseness = 1;
    QPainter mPainter;
    RenderType mRenderType = smoke;

public slots:
    void toggleShowVelocity(const bool visible);
    void setRenderType(const RenderType renderType) {this->mRenderType = renderType;}
};

#endif // RENDERENGINE_H
