cmake_minimum_required(VERSION 3.28)

project(vulkan-sycl-display)

# Enable generation of compile_commands.json for use with tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- C Language Settings ---
set(CMAKE_C_STANDARD 17)

# --- C++ Language Settings ---
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
include(FeatureSummary)

# --- Wayland Protocols Configuration ---
FetchContent_Declare(
  wayland-protocols
  GIT_REPOSITORY https://gitlab.freedesktop.org/wayland/wayland-protocols.git
  GIT_TAG 1.34)
FetchContent_MakeAvailable(wayland-protocols)
set(XDG_SHELL_PROTOCOL ${wayland-protocols_SOURCE_DIR}/stable/xdg-shell/xdg-shell.xml)
set(ZWP_TABLET_V2_PROTOCOL ${wayland-protocols_SOURCE_DIR}/unstable/tablet/tablet-unstable-v2.xml)
set(ZWP_LINUX_DMABUF_V1_PROTOCOL ${wayland-protocols_SOURCE_DIR}/stable/linux-dmabuf/linux-dmabuf-v1.xml)

message(XDG_SHELL_PROTOCOL="${XDG_SHELL_PROTOCOL}")
message(ZWP_TABLET_V2_PROTOCOL="${ZWP_TABLET_V2_PROTOCOL}")
message(ZWP_LINUX_DMA_BUF_V1="${ZWP_LINUX_DMABUF_V1_PROTOCOL}")
add_custom_command(
  OUTPUT xdg-shell-private.c
  COMMAND wayland-scanner client-header ${XDG_SHELL_PROTOCOL} xdg-shell-client.h
  COMMAND wayland-scanner private-code  ${XDG_SHELL_PROTOCOL} xdg-shell-private.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
  OUTPUT zwp-tablet-v2-private.c
  COMMAND wayland-scanner client-header ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-client.h
  COMMAND wayland-scanner private-code  ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-private.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
  OUTPUT zwp-linux-dmabuf-v1-private.c
  COMMAND wayland-scanner client-header ${ZWP_LINUX_DMABUF_V1_PROTOCOL} zwp-linux-dmabuf-v1-client.h
  COMMAND wayland-scanner private-code  ${ZWP_LINUX_DMABUF_V1_PROTOCOL} zwp-linux-dmabuf-v1-private.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Declare Catch2 as an external dependency using FetchContent
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG v3.11.0)
FetchContent_MakeAvailable(catch2)

# --- host platform (library) ---
add_library(host-utils STATIC
  # wayland-coroutines.cc # To be added later
  # vulkan-swapchain.cc   # To be added later
  ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-private.c
  ${CMAKE_CURRENT_BINARY_DIR}/zwp-tablet-v2-private.c
  ${CMAKE_CURRENT_BINARY_DIR}/zwp-linux-dmabuf-v1-private.c)
target_compile_options(host-utils PRIVATE
  -Wall
  -Wextra)

# --- device platform (main target) ---
add_executable(vulkan-sycl-display
  # prerender.cc        # To be added later
  main.cc)
target_compile_options(vulkan-sycl-display PRIVATE
  -Wall
  -Wextra
  -fsycl
  $<IF:$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","IntelLLVM">, "-fsycl-targets=spir64", "">
  $<IF:$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","Clang">, "-fsycl-targets=nvptx64-nvidia-cuda", "">
)
target_include_directories(vulkan-sycl-display
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}  # For generated headers
  ${CMAKE_CURRENT_SOURCE_DIR}) # For aux headers
target_link_directories(vulkan-sycl-display
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/lib/x86_64-linux-gnu)
target_link_libraries(vulkan-sycl-display PRIVATE
  host-utils
  vulkan
  wayland-client)

# --- test target ---
file(GLOB_RECURSE TST *-test.cc)
list(FILTER TST EXCLUDE REGEX "build/.*")
enable_testing()
add_executable(vulkan-sycl-display-test ${TST})
target_compile_options(vulkan-sycl-display-test PRIVATE
  -Wall
  -Wextra
  -fsycl
  $<IF:$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","IntelLLVM">, "-fsycl-targets=spir64", "">
  $<IF:$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","Clang">, "-fsycl-targets=nvptx64-nvidia-cuda", "">
)

target_include_directories(vulkan-sycl-display-test PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vulkan-sycl-display-test PRIVATE Catch2::Catch2WithMain)
add_test(NAME run_criss_cross_tests COMMAND vulkan-sycl-display-test)

# --- run in develop ---
add_custom_target(run
  DEPENDS vulkan-sycl-display vulkan-sycl-display-test
  COMMAND ctest --verbose && WAYLAND_DEBUG=1 ./vulkan-sycl-display)
