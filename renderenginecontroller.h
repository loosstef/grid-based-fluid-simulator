#ifndef RENDERENGINECONTROLLER_H
#define RENDERENGINECONTROLLER_H

#include <QObject>

class RenderEngine;
class QCheckBox;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;


class RenderEngineController : public QObject
{
    Q_OBJECT
public:
    RenderEngineController(RenderEngine* renderEngine);
    void connectView(QCheckBox* toggleVelocityCheckBox, QDoubleSpinBox* velocityScale, QSpinBox* velocitySparseness);
    void connectRenderTypeSelector(QComboBox* renderTypeSelector);
private:
    RenderEngine* mRenderEngine;
public slots:
    void toggleVelocityVisibility(int state);
    void changeVelocityScale(double scale);
    void changeVelocitySparseness(int sparseness);
    void renderTypeChanged(int newRenderTypeIndex);
};

#endif // RENDERENGINECONTROLLER_H
