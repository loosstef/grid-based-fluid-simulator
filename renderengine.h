#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QImage>
#include "field.h"
#include "renderenginecontroller.h"

class RenderEngine
{
public:
    RenderEngine(int width, int height, bool showVelocity = false);
    QImage* render(Field* field);
    void showVelocity(bool showVelocity) {this->mShowVelocity = showVelocity;}
    RenderEngineController *getController();
    void setVelocityScale(float velScale);
private:
    void renderSmoke(QImage* image, Field* field);
    void renderVelocity(QImage* image, Field* field);
    float valueToColorIntensity(float value);
    int mWidth;
    int mHeight;
    bool mShowVelocity;
    float mVelocityScale = 1;
public slots:
    void toggleShowVelocity(bool visible);
};

#endif // RENDERENGINE_H
