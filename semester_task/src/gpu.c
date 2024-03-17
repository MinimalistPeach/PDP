#include "gpu.h"

int start_gpu_particle_updates(int NUM_PARTICLES, float dt, Particle *particles, float *randoms, int randX, int randY, int randVX, int randVY)
{
    int i;
    cl_int err;
    int error_code;

    cl_mem particles_buffer, randoms_buffer, dt_buffer, randX_buffer, randY_buffer, randVX_buffer, randVY_buffer, NUM_PARTICLES_buffer;

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
    const char *kernel_code = load_kernel_source("kernels/update_particles.cl", &error_code);
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
    cl_kernel kernel = clCreateKernel(program, "update_particles", NULL);

    particles_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Particle) * NUM_PARTICLES, particles, &err);
    randoms_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 2 * NUM_PARTICLES, randoms, &err);
    dt_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * dt, &dt, &err);
    randX_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * randX, &randX, &err);
    randY_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * randY, &randY, &err);
    randVX_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * randVX, &randVX, &err);
    randVY_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * randVY, &randVY, &err);
    NUM_PARTICLES_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * NUM_PARTICLES, &NUM_PARTICLES, &err);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &particles_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &randoms_buffer);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &dt_buffer);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &randX_buffer);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &randY_buffer);
    clSetKernelArg(kernel, 5, sizeof(cl_mem), &randVX_buffer);
    clSetKernelArg(kernel, 6, sizeof(cl_mem), &randVY_buffer);
    clSetKernelArg(kernel, 7, sizeof(cl_mem), &NUM_PARTICLES_buffer);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    size_t global_size = 128;
    cl_event event;
    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, &event);

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
    printf("GPU Runtime: %lu ms\n", (endNs - startNs) / 1000000);

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);
    clReleaseMemObject(particles_buffer);
    clReleaseMemObject(randoms_buffer);
    clReleaseCommandQueue(command_queue);
}