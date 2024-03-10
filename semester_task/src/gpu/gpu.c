#include "gpu.h"
const int SAMPLE_SIZE = 1000;
void gpu_start(Node *nodes, int num_nodes)
{

    int i;
    cl_int err;
    int error_code;

    // Get platform
    cl_uint n_platforms;
    cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
        return 0;
    }

    // Get device
    cl_device_id device_id;
    cl_uint n_devices;
    err = clGetDeviceIDs(
        platform_id,
        CL_DEVICE_TYPE_GPU,
        1,
        &device_id,
        &n_devices);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
        return 0;
    }

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char *kernel_code = load_kernel_source("kernels/bfs.cl", &error_code);
    if (error_code != 0)
    {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    const char options[] = "";
    err = clBuildProgram(
        program,
        1,
        &device_id,
        options,
        NULL,
        NULL);
    if (err != CL_SUCCESS)
    {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size);
        char *build_log = (char *)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size);
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "bfs", NULL);

    // Create the device buffer
    cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(Node), NULL, NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(Node), (void *)&nodes);
    clSetKernelArg(kernel, 1, sizeof(int), (void *)&num_nodes);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        device_buffer,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(cl_float4),
        nodes,
        0,
        NULL,
        NULL);

    // Size specification
    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    // Apply the kernel on the range
    cl_event event;
    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        &event);
    clFinish(command_queue);

    // Show profiling information
    cl_ulong startNs;
    cl_ulong endNs;
    err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_QUEUED,
        sizeof(startNs),
        &startNs,
        NULL);
    if (err == CL_PROFILING_INFO_NOT_AVAILABLE)
    {
        printf("Profiling info not available!\n");
        return 0;
    }
    else if (err != CL_SUCCESS)
    {
        printf("Error code: %d\n", err);
        return 0;
    }
    clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(endNs),
        &endNs,
        NULL);
    printf("GPU runtime    : %lu ms\n", (endNs - startNs) / 1000);

    // Host buffer <- Device buffer
    clEnqueueReadBuffer(
        command_queue,
        device_buffer,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(cl_float4),
        nodes,
        0,
        NULL,
        NULL);

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(nodes);
}