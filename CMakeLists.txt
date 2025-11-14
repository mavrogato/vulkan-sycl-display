
cmake_minimum_required(VERSION 3.28)

include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall -Wextra -save-temps")

# C++
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fsycl -fsycl-targets=nvptx64-nvidia-cuda -save-temps")

# wayland protocols
include(FeatureSummary)
set_package_properties(WaylandProtocols PROPERTIES
  URL "https://cgit.freedesktop.org/wayland/wayland-protocols"
  DESCRIPTION "current wayland protocols")
unset(WAYLAND_PROTOCOLS_PATH)
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=pkgdatadir wayland-protocols
    OUTPUT_VARIABLE WAYLANDPROTOCOLS_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif ()
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WaylandProtocols DEFAULT_MSG WAYLANDPROTOCOLS_PATH)
mark_as_advanced(wAYLANDPROTOCOLS_PATH)
set(XDG_SHELL_PROTOCOL     ${WAYLANDPROTOCOLS_PATH}/stable/xdg-shell/xdg-shell.xml)
set(ZWP_TABLET_V2_PROTOCOL ${WAYLANDPROTOCOLS_PATH}/unstable/tablet/tablet-unstable-v2.xml)
set(ZWP_LINUX_DMABUF_V1_PROTOCOL    ${WAYLANDPROTOCOLS_PATH}/stable/linux-dmabuf/linux-dmabuf-v1.xml)
message(XDG_SHELL_PROTOCOL="${XDG_SHELL_PROTOCOL}")
message(ZWP_TABLET_V2_PROTOCOL="${ZWP_TABLET_V2_PROTOCOL}")
message(ZWP_LINUX_DMA_BUF_V1="${ZWP_LINUX_DMABUF_V1_PROTOCOL}")
add_custom_command(
  OUTPUT xdg-shell-private.c
  COMMAND wayland-scanner client-header ${XDG_SHELL_PROTOCOL} xdg-shell-client.h
  COMMAND wayland-scanner private-code  ${XDG_SHELL_PROTOCOL} xdg-shell-private.c)
add_custom_command(
  OUTPUT zwp-tablet-v2-private.c
  COMMAND wayland-scanner client-header ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-client.h
  COMMAND wayland-scanner private-code  ${ZWP_TABLET_V2_PROTOCOL} zwp-tablet-v2-private.c)
add_custom_command(
  OUTPUT zwp-linux-dmabuf-v1-private.c
  COMMAND wayland-scanner client-header ${ZWP_LINUX_DMABUF_V1_PROTOCOL} zwp-linux-dmabuf-v1-client.h
  COMMAND wayland-scanner private-code  ${ZWP_LINUX_DMABUF_V1_PROTOCOL} zwp-linux-dmabuf-v1-private.c)

# build steps
project(vulkan-sycl-display)

add_executable(vulkan-sycl-display
  main.cc
  ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-private.c
  ${CMAKE_CURRENT_BINARY_DIR}/zwp-tablet-v2-private.c
  ${CMAKE_CURRENT_BINARY_DIR}/zwp-linux-dmabuf-v1-private.c)

target_include_directories(vulkan-sycl-display
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_directories(vulkan-sycl-display
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/lib/x86_64-linux-gnu)

target_link_libraries(vulkan-sycl-display PRIVATE
  vulkan
  wayland-client)

# Check2 test framework
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG v3.11.0)
FetchContent_MakeAvailable(catch2)
file(GLOB_RECURSE TST *-test.cc)
list(FILTER TST EXCLUDE REGEX "build/.*")
enable_testing()
add_executable(vulkan-sycl-display-test ${TST})
target_include_directories(vulkan-sycl-display-test PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vulkan-sycl-display-test PRIVATE Catch2::Catch2WithMain)
add_test(NAME run_criss_cross_tests COMMAND vulkan-sycl-display-test)

add_custom_target(run
  DEPENDS vulkan-sycl-display vulkan-sycl-display-test
  COMMAND ctest --verbose && WAYLAND_DEBUG=1 ./vulkan-sycl-display)
