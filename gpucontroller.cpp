#include "gpucontroller.h"

#include <iostream>
#include <QFile>
#include <QTextStream>
#include "grid.h"

GpuConnector* singleton = nullptr;

GpuConnector::GpuConnector()
{
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

    context = cl::Context({default_device});

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    QFile f(":/opencl/simulation_steps.cl");
       if (!f.open(QFile::ReadOnly | QFile::Text)) exit(400);
       QTextStream in(&f);
    std::string kernel_code = in.readAll().toStdString();
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    program = cl::Program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }
    std::cout<<"Done building\n";

    //create queue to which we will push commands for the device.
    queue = cl::CommandQueue(context,default_device);

    kernel_forward_advection=cl::Kernel(program,"calc_9_grids_for_forward_advection");
    kernel_sum_9_arrays=cl::Kernel(program,"sum_9_arrays");
}

GpuConnector *GpuConnector::get()
{
    if(singleton == nullptr) {
        singleton = new GpuConnector();
    }
    return singleton;
}

void GpuConnector::calc_9_grids_for_forward_advection(int edgeCase, Grid *horVel, Grid *verVel, Grid *sourceValues, float *grid_1, float *grid_2, float *grid_3, float *grid_4, float *grid_5, float *grid_6, float *grid_7, float *grid_8, float *grid_9)
{
    // create buffers
    cl::Buffer buffer_width = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int));
    cl::Buffer buffer_height = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int));
    cl::Buffer buffer_edgeCase = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(int));
    cl::Buffer buffer_horVel = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*horVel->getSize());
    cl::Buffer buffer_verVel = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*verVel->getSize());
    cl::Buffer buffer_sourceValues = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_1 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_2 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_3 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_4 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_5 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_6 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_7 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_8 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());
    cl::Buffer buffer_grid_9 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*sourceValues->getSize());

    int width = sourceValues->getWidth();
    int height = sourceValues->getHeight();

    // write data to buffers
    queue.enqueueWriteBuffer(buffer_width,CL_TRUE,0,sizeof(int), &width);
    queue.enqueueWriteBuffer(buffer_height,CL_TRUE,0,sizeof(int), &height);
    queue.enqueueWriteBuffer(buffer_edgeCase,CL_TRUE,0,sizeof(int), &edgeCase);
    queue.enqueueWriteBuffer(buffer_horVel,CL_TRUE,0,sizeof(float)*horVel->getSize(), horVel->getRaw());
    queue.enqueueWriteBuffer(buffer_verVel,CL_TRUE,0,sizeof(float)*verVel->getSize(), verVel->getRaw());
    queue.enqueueWriteBuffer(buffer_sourceValues,CL_TRUE,0,sizeof(float)*sourceValues->getSize(), sourceValues->getRaw());

    // run the kernel
    kernel_forward_advection.setArg(0, buffer_width);
    kernel_forward_advection.setArg(1, buffer_height);
    kernel_forward_advection.setArg(2, buffer_edgeCase);
    kernel_forward_advection.setArg(3, buffer_horVel);
    kernel_forward_advection.setArg(4, buffer_verVel);
    kernel_forward_advection.setArg(5, buffer_sourceValues);
    queue.enqueueNDRangeKernel(kernel_forward_advection,cl::NullRange,cl::NDRange(sourceValues->getSize()),cl::NullRange);
    queue.finish();

    // read result
    queue.enqueueReadBuffer(buffer_grid_1,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_1);
    queue.enqueueReadBuffer(buffer_grid_2,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_2);
    queue.enqueueReadBuffer(buffer_grid_3,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_3);
    queue.enqueueReadBuffer(buffer_grid_4,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_4);
    queue.enqueueReadBuffer(buffer_grid_5,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_5);
    queue.enqueueReadBuffer(buffer_grid_6,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_6);
    queue.enqueueReadBuffer(buffer_grid_7,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_7);
    queue.enqueueReadBuffer(buffer_grid_8,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_8);
    queue.enqueueReadBuffer(buffer_grid_9,CL_TRUE,0,sizeof(float)*sourceValues->getSize(),grid_9);
}

void GpuConnector::sumate_9_arrays(float *result, int size, float *grid_1, float *grid_2, float *grid_3, float *grid_4, float *grid_5, float *grid_6, float *grid_7, float *grid_8, float *grid_9)
{
    // create buffers
    cl::Buffer buffer_target = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_1 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_2 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_3 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_4 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_5 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_6 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_7 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_8 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);
    cl::Buffer buffer_grid_9 = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*size);

    // write data to buffers
    queue.enqueueWriteBuffer(buffer_grid_1,CL_TRUE,0,sizeof(float)*size, grid_1);
    queue.enqueueWriteBuffer(buffer_grid_2,CL_TRUE,0,sizeof(float)*size, grid_2);
    queue.enqueueWriteBuffer(buffer_grid_3,CL_TRUE,0,sizeof(float)*size, grid_3);
    queue.enqueueWriteBuffer(buffer_grid_4,CL_TRUE,0,sizeof(float)*size, grid_4);
    queue.enqueueWriteBuffer(buffer_grid_5,CL_TRUE,0,sizeof(float)*size, grid_5);
    queue.enqueueWriteBuffer(buffer_grid_6,CL_TRUE,0,sizeof(float)*size, grid_6);
    queue.enqueueWriteBuffer(buffer_grid_7,CL_TRUE,0,sizeof(float)*size, grid_7);
    queue.enqueueWriteBuffer(buffer_grid_8,CL_TRUE,0,sizeof(float)*size, grid_8);
    queue.enqueueWriteBuffer(buffer_grid_9,CL_TRUE,0,sizeof(float)*size, grid_9);

    // run kernel
    kernel_sum_9_arrays.setArg(0, buffer_grid_1);
    kernel_sum_9_arrays.setArg(1, buffer_grid_2);
    kernel_sum_9_arrays.setArg(2, buffer_grid_3);
    kernel_sum_9_arrays.setArg(3, buffer_grid_4);
    kernel_sum_9_arrays.setArg(4, buffer_grid_5);
    kernel_sum_9_arrays.setArg(5, buffer_grid_6);
    kernel_sum_9_arrays.setArg(6, buffer_grid_7);
    kernel_sum_9_arrays.setArg(7, buffer_grid_8);
    kernel_sum_9_arrays.setArg(8, buffer_grid_9);
    queue.enqueueNDRangeKernel(kernel_sum_9_arrays,cl::NullRange,cl::NDRange(size),cl::NullRange);
    queue.finish();

    // read result
    queue.enqueueReadBuffer(buffer_target,CL_TRUE,0,sizeof(float)*size,result);

}

/*Grid *GpuConnector::copyGrid(Grid *srcGrid)
{
    // create buffers on the device
    buffer_source = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*srcGrid->getSize());
    buffer_target = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*srcGrid->getSize());

    float* sourceValues = srcGrid->getRaw();

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_source,CL_TRUE,0,sizeof(float)*srcGrid->getSize(), sourceValues);

    //run the kernel
    kernel_copy_array.setArg(0,buffer_source);
    kernel_copy_array.setArg(1,buffer_target);
    queue.enqueueNDRangeKernel(kernel_copy_array,cl::NullRange,cl::NDRange(srcGrid->getSize()),cl::NullRange);
    queue.finish();

    float* targetValues = new float[srcGrid->getSize()];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_target,CL_TRUE,0,sizeof(float)*srcGrid->getSize(),targetValues);

    return new Grid(targetValues, srcGrid->getWidth(), srcGrid->getHeight());
}*/
