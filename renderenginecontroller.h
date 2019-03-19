#ifndef RENDERENGINECONTROLLER_H
#define RENDERENGINECONTROLLER_H

#include <QObject>
#include <QCheckBox>

class RenderEngine;


class RenderEngineController : public QObject
{
    Q_OBJECT
public:
    RenderEngineController(RenderEngine* renderEngine);
    void connectView(QCheckBox* toggleVelocityCheckBox);
private:
    RenderEngine* mRenderEngine;
public slots:
    void toggleVelocityVisibility(int state);
};

#endif // RENDERENGINECONTROLLER_H
