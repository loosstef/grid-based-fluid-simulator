#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>

class QImage;
class Field;
class RenderEngineController;

class RenderEngine : public QObject
{
    Q_OBJECT
public:
    RenderEngine(const int width, const int height, const bool showVelocity = false);
    QImage* render(const Field* field) const;
    void showVelocity(const bool showVelocity) {this->mShowVelocity = showVelocity;}
    RenderEngineController *getController();
    void setVelocityScale(const float velScale);
private:
    void renderSmoke(QImage* image, const Field* field) const;
    void renderVelocity(QImage* image, const Field* field) const;
    float valueToColorIntensity(float value) const;
    int mWidth;
    int mHeight;
    bool mShowVelocity;
    float mVelocityScale = 1;
public slots:
    void toggleShowVelocity(const bool visible);
};

#endif // RENDERENGINE_H
