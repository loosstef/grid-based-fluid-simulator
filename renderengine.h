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
    void setVelocityScale(const float velScale);
    void setVelocitySparseness(const int velSparseness) {this->mVelocitySparseness = velSparseness;}
private:
    void renderGrid(QImage* image, const Grid* grid, float (*f)(float)) const;
    void renderWalls(QImage* image, Grid* walls, int width, int height, float xScale, float yScale);
    void renderVelocity(QImage* image, const Field* field);
    float valueToColorIntensity(float value, float (*f)(float)) const;
    static float smokeToColorIntensity(float value);
    static float densityToColorIntensity(float value);
    static float temperatureToColorIntensity(float value);
    static Grid *generatePressureGrid(Grid* mass, Grid* temp);
    int mWidth;
    int mHeight;
    bool mShowVelocity;
    float mVelocityScale = 1;
    int mVelocitySparseness = 1;
    QPainter mPainter;
    RenderType mRenderType = smoke;

public slots:
    void toggleShowVelocity(const bool visible);
    void setRenderType(const RenderType renderType) {this->mRenderType = renderType;}
};

#endif // RENDERENGINE_H
