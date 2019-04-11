#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QPainter>

class QImage;
class Field;
class RenderEngineController;

enum RenderType {smoke, density};

class RenderEngine : public QObject
{
    Q_OBJECT
public:
    RenderEngine(const int width, const int height, const bool showVelocity = false);
    QImage* render(const Field* field);
    void showVelocity(const bool showVelocity) {this->mShowVelocity = showVelocity;}
    RenderEngineController *getController();
    void setVelocityScale(const float velScale);
private:
    void renderSmoke(QImage* image, const Field* field) const;
    void renderDensity(QImage* image, const Field* field) const;
    void renderVelocity(QImage* image, const Field* field);
    float valueToColorIntensity(float value, float (*f)(float)) const;
    static float smokeToColorIntensity(float value);
    static float densityToColorIntensity(float value);
    int mWidth;
    int mHeight;
    bool mShowVelocity;
    float mVelocityScale = 1;
    QPainter mPainter;
    RenderType mRenderType = density;

public slots:
    void toggleShowVelocity(const bool visible);
};

#endif // RENDERENGINE_H
