#ifndef RENDERENGINECONTROLLER_H
#define RENDERENGINECONTROLLER_H

#include <QObject>
#include <QCheckBox>
#include <QDoubleSpinBox>

class RenderEngine;


class RenderEngineController : public QObject
{
    Q_OBJECT
public:
    RenderEngineController(RenderEngine* renderEngine);
    void connectView(QCheckBox* toggleVelocityCheckBox, QDoubleSpinBox* velocityScale);
private:
    RenderEngine* mRenderEngine;
public slots:
    void toggleVelocityVisibility(int state);
    void changeVelocityScale(double scale);
};

#endif // RENDERENGINECONTROLLER_H
