cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(FindHDF5   REQUIRED)
include(FindOpenCL REQUIRED)

add_executable(fdtds-opencl
    src/common/common_helpers_internal.h
    src/common/common_typedefs.h
    src/common/error_handling.c
    src/common/error_handling.h
    src/common/memory_allocation.c
    src/common/memory_allocation.h
    src/common/simulation_parameters_enums.h
    src/common/simulation_parameters.h
    src/common/simulation_parameters_reader.c
    src/common/simulation_parameters_reader.h
    src/opencl/main.c)

target_include_directories(fdtds-opencl
  PUBLIC src/common
  PUBLIC src/opencl
  PUBLIC "${HDF5_INCLUDE_DIRS}"
  PUBLIC "${OpenCL_INCLUDE_DIRS}")

target_link_libraries(fdtds-opencl
  "${HDF5_C_LIBRARIES}"
  "${OpenCL_LIBRARIES}")
