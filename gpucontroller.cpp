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
    QFile f(":/opencl/copy_array.cl");
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

    kernel_copy_array=cl::Kernel(program,"copy_array");
}

GpuConnector *GpuConnector::get()
{
    if(singleton == nullptr) {
        singleton = new GpuConnector();
    }
    return singleton;
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
