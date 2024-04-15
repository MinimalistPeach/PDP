#include "gpu.h"

int startGpuParticleUpdates(int numParticles, float dt, 
                                Particle *particles, float *randoms, 
                                int randX, int randY, 
                                int randVX, int randVY)
{
    int i;
    cl_int err;
    int errorCode;

    cl_mem particlesBuffer, randomsBuffer, 
        dtBuffer, randXBuffer, randYBuffer, 
        randVXBuffer, randVYBuffer, numParticlesBuffer;

    // Get platform
    cl_uint nPlatforms;
    cl_platform_id platformId;
    err = clGetPlatformIDs(1, &platformId, &nPlatforms);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
        return 0;
    }

    // Get device
    cl_device_id deviceId;
    cl_uint nDevices;
    err = clGetDeviceIDs(
        platformId,
        CL_DEVICE_TYPE_GPU,
        1,
        &deviceId,
        &nDevices);
    if (err != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
        return 0;
    }

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, nDevices, &deviceId, NULL, NULL, NULL);

    // Build the program
    const char *kernelCode = load_kernel_source("kernels/update_particles.cl", &errorCode);
    if (errorCode != 0)
    {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernelCode, NULL, NULL);
    const char options[] = "";
    err = clBuildProgram(
        program,
        1,
        &deviceId,
        options,
        NULL,
        NULL);
    if (err != CL_SUCCESS)
    {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            deviceId,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size);
        char *build_log = (char *)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            deviceId,
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
    cl_kernel kernel = clCreateKernel(program, "update_particles", NULL);

    particlesBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Particle) * numParticles, particles, &err);
    randomsBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 2 * numParticles, randoms, &err);

    clock_t data_transfer_start = clock();
    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &particlesBuffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &randomsBuffer);
    clSetKernelArg(kernel, 2, sizeof(float), &dt);
    clSetKernelArg(kernel, 3, sizeof(int), &randX);
    clSetKernelArg(kernel, 4, sizeof(int), &randY);
    clSetKernelArg(kernel, 5, sizeof(int), &randVX);
    clSetKernelArg(kernel, 6, sizeof(int), &randVY);
    clSetKernelArg(kernel, 7, sizeof(int), &numParticles);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, deviceId, CL_QUEUE_PROFILING_ENABLE, NULL);

    size_t global_size = 1024;
    cl_event event;
    clEnqueueNDRangeKernel(command_queue, kernel, 0, NULL, &global_size, NULL, 0, NULL, &event);

    clFinish(command_queue);

    // Show profiling information
    cl_ulong startMs;
    cl_ulong endMs;
    err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_QUEUED,
        sizeof(startMs),
        &startMs,
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
        sizeof(endMs),
        &endMs,
        NULL);
    clock_t data_transfer_end = clock();
    printf("GPU Runtime: %lu ms\n", (endMs - startMs) / 1000000);
    printf("Data transfer time: %lu ms\n", ((endMs - startMs) / 1000000) - (double)(data_transfer_end - data_transfer_start));

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(deviceId);
    clReleaseMemObject(particlesBuffer);
    clReleaseMemObject(randomsBuffer);
    clReleaseCommandQueue(command_queue);
}