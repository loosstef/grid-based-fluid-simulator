#ifndef PAINTHANDLER_H
#define PAINTHANDLER_H

#include <QObject>

class QSpinBox;
class QDoubleSpinBox;

class Field;
class PaintTool;
class PaintToolController;
class SimulationViewer;

class PaintHandler : public QObject
{
    Q_OBJECT
public:
    PaintHandler(Field* field);
    void connectToSimulationViewer(SimulationViewer* simViewer);
    void connectToSettings(QSpinBox* brushSize, QDoubleSpinBox* hardness);
private:
    PaintTool* mSmokePaintTool;
    PaintToolController* mSmokePaintToolController;
public slots:
    void leftClicked(int x, int y);
    void leftMoved(int x, int y);
    void rightClicked(int x, int y);
    void rightMoved(int x, int y);
};

#endif // PAINTHANDLER_H
