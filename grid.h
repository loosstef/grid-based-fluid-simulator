#ifndef GRID_H
#define GRID_H

class Grid
{
public:
    Grid(const int width, const int height, const double initValue = 0);
    Grid(double* values, const int width, const int height);
    ~Grid();
    int getWidth() const {return this->width;}
    int getHeight() const {return this->height;}
    double get(int x, int y) const;
    double get(int i) const {return this->values[i];}
    const double *getRaw() const {return this->values;}
    int getSize() const {return width*height;}
    bool set(const int x, const int y, const double value);
    void set(double* values);
    bool add(const int x, const int y, const double value);
    void reset(const double val);
    static Grid *deepCopy(const Grid* grid);
private:
    int width;
    int height;
    double* values;
};

#endif // GRID_H
