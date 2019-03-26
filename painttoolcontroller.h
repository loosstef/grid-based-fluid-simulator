#ifndef PAINTTOOLCONTROLLER_H
#define PAINTTOOLCONTROLLER_H

#include <QObject>
#include <QSpinBox>
#include <QDoubleSpinBox>

class PaintTool;


class PaintToolController : public QObject
{
    Q_OBJECT
public:
    PaintToolController(PaintTool* paintTool, QObject* parent = nullptr);
    void connectToBrushSettings(QSpinBox* brushSizeField, QDoubleSpinBox* brushHardnessField);
private:
    PaintTool* mPaintTool;
public slots:
    void brushSizeChanged(int i);
    void brushHardnessChanged(double i);
};

#endif // PAINTTOOLCONTROLLER_H
