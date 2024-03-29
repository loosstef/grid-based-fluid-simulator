#include "painttool.h"

PaintTool::PaintTool(Grid* grid) : mGrid(grid)
{

}

PaintToolController *PaintTool::getController()
{
    return new PaintToolController(this);
}

/**
 * Draw a point in a given grid
 * @brief PaintTool::drawPoint
 * @param grid the grid to draw on
 * @param x x-coordinate
 * @param y y-coordinate
 */
void PaintTool::drawPoint(int x, int y)
{
    drawPoint(x, y, mHardness);
}

void PaintTool::erasePoint(int x, int y)
{
    drawPoint(x, y, -mHardness);
}

void PaintTool::setPoint(int x, int y)
{
    int minXBound = x - (int)(this->mSize/2);
    int maxXBound = minXBound + this->mSize;
    int minYBound = y - (int)(this->mSize/2);
    int maxYBound = minYBound + this->mSize;
    for(int drawX = minXBound; drawX < maxXBound; ++drawX ) {
        for(int drawY = minYBound; drawY < maxYBound; ++drawY) {
            this->mGrid->set(drawX, drawY, mHardness);
        }
    }
}

void PaintTool::resetPoint(int x, int y)
{
    int minXBound = x - (int)(this->mSize/2);
    int maxXBound = minXBound + this->mSize;
    int minYBound = y - (int)(this->mSize/2);
    int maxYBound = minYBound + this->mSize;
    for(int drawX = minXBound; drawX < maxXBound; ++drawX ) {
        for(int drawY = minYBound; drawY < maxYBound; ++drawY) {
            this->mGrid->set(drawX, drawY, mMinValue);
        }
    }
}

void PaintTool::drawPoint(int x, int y, double value)
{
    int minXBound = x - (int)(this->mSize/2);
    int maxXBound = minXBound + this->mSize;
    int minYBound = y - (int)(this->mSize/2);
    int maxYBound = minYBound + this->mSize;
    for(int drawX = minXBound; drawX < maxXBound; ++drawX ) {
        for(int drawY = minYBound; drawY < maxYBound; ++drawY) {
            if(this->mGrid->get(drawX, drawY) + value < mMinValue) {
                this->mGrid->set(drawX, drawY, mMinValue);
            } else {
                this->mGrid->add(drawX, drawY, value);
            }
        }
    }
}
