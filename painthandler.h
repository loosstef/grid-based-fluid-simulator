#ifndef PAINTHANDLER_H
#define PAINTHANDLER_H

#include <QObject>

class QSpinBox;
class QDoubleSpinBox;
class QComboBox;

class Field;
class PaintTool;
class PaintToolController;
class SimulationViewer;
class VelocityManipulator;

enum PaintType {SMOKE, WALL, TEMPERATURE};

class PaintHandler : public QObject
{
    Q_OBJECT
public:
    PaintHandler(Field* field);
    ~PaintHandler();
    void connectToSimulationViewer(SimulationViewer* simViewer);
    void connectToSettings(QComboBox* brushType, QSpinBox* smokeSize, QDoubleSpinBox* smokeHardness, QSpinBox* wallSize, QDoubleSpinBox* wallHardness, QSpinBox* tempSize, QDoubleSpinBox* tempHardness);
    void setPaintType(PaintType paintType) {mPaintType = paintType;}
private:
    PaintTool* mSmokePaintTool;
    PaintTool* mWallPaintTool;
    PaintTool* mTemperaturePainttool;
    PaintToolController* mSmokePaintToolController;
    PaintToolController* mWallPaintToolController;
    PaintToolController* mTemperaturePaintToolController;
    VelocityManipulator* mVelocityManipulator;
    PaintType mPaintType = SMOKE;
    int mLastSimX = 0;
    int mLastSimY = 0;
    int mLastViewX = 0;
    int mLastViewY = 0;
    qint64 mTimer;
public slots:
    void leftClicked(int x, int y);
    void leftMoved(int x, int y);
    void rightMouseClick(int simX, int simY, int viewX, int viewY);
    void rightMouseMove(int simX, int simY, int viewX, int viewY);
    void paintTypeChanged(int value);
};

#endif // PAINTHANDLER_H
