#ifndef PAINTTOOL_H
#define PAINTTOOL_H

#include <QObject>
#include "grid.h"
#include "painttoolcontroller.h"


class PaintTool : public QObject
{
    Q_OBJECT
public:
    PaintTool(Grid* grid);
    void setSize(int size) {this->mSize = size;}
    void setHardness(float hardness) {this->mHardness = hardness;}
    void setGrid(Grid* grid) {this->mGrid = grid;}
    PaintToolController *getController();
public slots:
    void drawPoint(int x, int y);
    void resetPoint(int x, int y);
private:
    int mSize = 1;
    float mHardness = 1;
    Grid* mGrid = nullptr;
};

#endif // PAINTTOOL_H
