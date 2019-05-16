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
    void setHardness(double hardness) {this->mHardness = hardness;}
    void setMinValue(double minValue) {this->mMinValue = minValue;}
    void setGrid(Grid* grid) {this->mGrid = grid;}
    PaintToolController *getController();
public slots:
    void drawPoint(int x, int y);
    void erasePoint(int x, int y);
    void setPoint(int x, int y);
    void resetPoint(int x, int y);
private:
    void drawPoint(int x, int y, double value);
    int mSize = 1;
    double mHardness = 1;
    double mMinValue = -99999;
    Grid* mGrid = nullptr;
};

#endif // PAINTTOOL_H
