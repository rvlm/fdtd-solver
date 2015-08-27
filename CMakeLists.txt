cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(FindHDF5   REQUIRED)
include(FindOpenCL REQUIRED)

add_executable(fdtds-opencl
  src/opencl/main.c)

target_include_directories(fdtds-opencl
  INTERFACE src/common
  INTERFACE src/opencl
  PUBLIC    "${HDF5_INCLUDE_DIRS}"
  PUBLIC    "${OpenCL_INCLUDE_DIRS}")

target_link_libraries(fdtds-opencl
  "${HDF5_C_LIBRARIES}"
  "${OpenCL_LIBRARIES}")
