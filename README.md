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

*   **Recommended:** Intel oneAPI Base Toolkit (tested with version YYYY.X)
*   **Alternatively:** A self-built Intel/llvm environment (e.g., as used by the author: `clang version 20.0.0git [...] InstalledDir: /home/mavrogato/sycl_workspace/llvm/build/bin`)
*   CMake (version 3.28 or newer)
*   [Other dependencies like Vulkan SDK, Catch2...]

## Build Instructions

This project requires a DPC++/C++ Compiler (icx/dpcpp) environment.



### Prerequisites

*   **Recommended Environment:** Intel oneAPI Base Toolkit (tested with version YYYY.X). This is the supported configuration.
*   **Author's Environment:** The author is using a highly customized, self-built Intel/llvm environment targeting NVIDIA GPUs on Wayland. **This configuration is complex and not recommended for general use.**

*   CMake (version 3.28 or newer)
*   **Vulkan SDK:** Installed and discoverable by CMake.
*   **Wayland Development Libraries:**
    *   **Ubuntu/Debian:** `libwayland-dev`
    *   **Fedora/CentOS:** `wayland-devel`
*   [Other dependencies like Catch2 (handled by FetchContent)...]

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
