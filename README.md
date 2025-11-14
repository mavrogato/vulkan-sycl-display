# vulkan-sycl-display

This repository contains a prototype GPGPU rendering system using Intel/llvm (Sycl).

## Project Goals

*   **Objective**: Prototyping a GPGPU rendering system utilizing Intel/llvm (Sycl).
*   **Environment**: Development environment is Ubuntu Wayland on a system with an NVIDIA GPU (specifically using Sycl, not CUDA).
*   **Target**: Achieving interoperability between `wl_drm`, Vulkan, and Sycl.

## Current Status

We are currently focusing on achieving initial interoperability between Vulkan and Sycl. The codebase is actively evolving.

## Contributing

Contribution guidelines are not yet finalized as the project is in its early stages. Specific instructions will be provided here in the future.

## Build Instructions

This project requires a DPC++/C++ Compiler (icx/dpcpp) environment.

### Prerequisites
*   **Recommended Environment:** Intel oneAPI Base Toolkit (tested with version 2024.2). This is the supported configuration. 
*   **Alternatively:** you can build https://github.com/intel/llvm by yourself.
*   CMake (version 3.28 or newer)
*   **Vulkan SDK:** Installed and discoverable by CMake.
*   **Wayland Development Libraries & Tools:**
    *   `libwayland-dev` (Ubuntu/Debian) or `wayland-devel` (Fedora/CentOS)
    *   `wayland-scanner` tool (usually included with the above packages, or installed separately as `wayland-protocols-devel`). 
*   Other dependencies (e.g., Catch2) are handled by CMake's FetchContent.

You may need to install additional system packages depending on your specific Linux distribution and setup.

### Building (General steps)

1.  **Ensure compilers are in your PATH:**
    Make sure that `icx` and `icpx` are accessible in your terminal's PATH. If using the recommended oneAPI toolkit, you typically source the environment script:
    ```bash
    source /opt/intel/oneapi/setvars.sh
    ```

2.  **Configure the build using CMake:**
    ```bash
    mkdir build
    cd build
    cmake .. -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
    ```

3.  **Build the project:**
    ```bash
    cmake --build .
    ```

## License

This project is licensed under the [MIT License](LICENSE).
