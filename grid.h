#ifndef GRID_H
#define GRID_H

class Grid
{
public:
    Grid(const int width, const int height, const float initValue = 0);
    Grid(float* values, const int width, const int height);
    ~Grid();
    int getWidth() const {return this->width;}
    int getHeight() const {return this->height;}
    float get(int x, int y) const;
    float get(int i) const {return this->values[i];}
    const float *getRaw() const {return this->values;}
    int getSize() const {return width*height;}
    bool set(const int x, const int y, const float value);
    void set(float* values);
    bool add(const int x, const int y, const float value);
    void reset(const float val);
    static Grid *deepCopy(const Grid* grid);
private:
    int width;
    int height;
    float* values;
};

#endif // GRID_H
