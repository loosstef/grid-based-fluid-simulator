#ifndef CPU_H
#define CPU_H

#include <CL/cl.hpp>

class Grid;

class GpuConnector
{
public:
    static GpuConnector *get();
    //Grid *copyGrid(Grid* srcGrid);
    void calc_9_grids_for_forward_advection(int edgeCase, Grid* horVel, Grid* verVel, Grid* sourceValues, float* grid_1, float* grid_2, float* grid_3, float* grid_4, float* grid_5, float* grid_6, float* grid_7, float* grid_8, float* grid_9);
    void sumate_9_arrays(float* result, int size, float* grid_1, float* grid_2, float* grid_3, float* grid_4, float* grid_5, float* grid_6, float* grid_7, float* grid_8, float* grid_9);
private:
    GpuConnector();
    cl::Device default_device;
    cl::Context context;
    cl::Program program;
    cl::CommandQueue queue;
    cl::Kernel kernel_forward_advection;
    cl::Kernel kernel_sum_9_arrays;
};

#endif // CPU_H
