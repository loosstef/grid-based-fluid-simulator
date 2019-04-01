#ifndef CPU_H
#define CPU_H

#include <CL/cl.hpp>

class Grid;

class GpuConnector
{
public:
    static GpuConnector *get();
    //Grid *copyGrid(Grid* srcGrid);
private:
    GpuConnector();
    cl::Device default_device;
    cl::Context context;
    cl::Program program;
    cl::Buffer buffer_source;
    cl::Buffer buffer_target;
    cl::CommandQueue queue;
    cl::Kernel kernel_copy_array;
};

#endif // CPU_H
