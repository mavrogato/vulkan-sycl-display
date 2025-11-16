markdown
# vulkan-sycl-display

This repository contains a prototype GPGPU rendering system using SYCL.

## Project Goals

*   **Objective**: Prototyping a GPGPU rendering system utilizing SYCL.
*   **Environment**: Development environment is Ubuntu Wayland on a system with an NVIDIA GPU.
*   **Target**: Achieving interoperability between `wl_drm`, Vulkan, and Sycl.

## Current Status

We are currently focusing on achieving initial interoperability between Vulkan and Sycl. The codebase is actively evolving.

## Contributing

Contribution guidelines are not yet finalized as the project is in its early stages. Specific instructions will be provided here in the future.

## Build Instructions

This project is intended to be highly portable and prioritizes using an open-source SYCL implementation. **AdaptiveCpp** is the recommended compiler environment.

### Prerequisites (Recommended: AdaptiveCpp Environment)

*   **Recommended Compiler:** **AdaptiveCpp** (formerly hipSYCL). This provides the best portability across different hardware vendors (NVIDIA, AMD, Intel).
    *   *Note: This requires a standard Clang/LLVM toolchain (e.g., Clang 18 on Ubuntu 24.04) as the frontend.*
*   CMake (version 3.28 or newer)
*   **Vulkan SDK:** Installed and discoverable by CMake.
*   **Wayland Development Libraries & Tools:**
    *   `libwayland-dev` (Ubuntu/Debian) or `wayland-devel` (Fedora/CentOS)
    *   `wayland-scanner` tool (usually included with the above packages).
*   **Hardware SDKs:**
    *   NVIDIA GPU users: NVIDIA CUDA Toolkit (for the AdaptiveCpp CUDA backend).
*   Other dependencies (e.g., Catch2) are handled by CMake's FetchContent.

You may need to install additional system packages depending on your specific Linux distribution and setup.

### Building (General steps using AdaptiveCpp)

1.  **Ensure compilers are in your PATH:**
    Make sure that `clang++` is accessible in your terminal's PATH. Ensure AdaptiveCpp is correctly installed and discoverable.

2.  **Configure the build using CMake:**
    *(AdaptiveCpp uses a standard Clang compiler flow)*
    ```bash
    mkdir build
    cd build
    # Set the SYCL runtime and target backend (e.g., CUDA)
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DWITH_ACCELERATORS=cuda
    ```

3.  **Build the project:**
    ```bash
    cmake --build .
    ```

### Alternative: Intel oneAPI DPC++/C++ Environment

For comparison or specific Intel hardware optimization, you can use the Intel oneAPI DPC++/C++ Compiler (icx/icpx). This environment is less portable but offers optimized performance on Intel hardware.

*   **Prerequisites:** Intel oneAPI Base Toolkit installed.
*   **Building:** Source `setvars.sh` and configure CMake:
    ```bash
    source /opt/intel/oneapi/setvars.sh
    mkdir build_intel
    cd build_intel
    cmake .. -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
    cmake --build .
    ```

## License

This project is licensed under the [MIT License](LICENSE).
