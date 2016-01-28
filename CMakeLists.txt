cmake_minimum_required(VERSION 2.8)
project(rFDTD C)

# Just shortcuts. They're short, nice and distinctive, so any path must be
# explicitly specified as for source directory or build directory (with some
# small exceptions like for 'install' command).
set(S "${CMAKE_CURRENT_SOURCE_DIR}")
set(B "${CMAKE_CURRENT_BINARY_DIR}")

# Thirdparty libraries lookup.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${S}")
find_package(NetCDF REQUIRED)
find_package(OpenCL REQUIRED)

# Build options.
option(RFDTD_CONFIG_USE_DOUBLE "Use double precision floats")
option(RFDTD_CONFIG_UNIT_TESTS "Build unit-tests for library")
option(RFDTD_CONFIG_CPPCHECK   "Check library sources with CppCheck")
option(RFDTD_CONFIG_VALGRIND   "Check library unit tests with Valgrind")
option(RFDTD_INSTALL_SOURCES   "Install source files too")

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

    # These are the prefered GCC options for *developer* environment. They try
    # to enable as much of useful warnings as possible while trying to keep
    # “signal-noise” ration in compiler output still acceptable. Please ensure
    # that your code builds with these settings without warnings BEFORE merging
    # it into master branch.
    # Originally taken from http://stackoverflow.com/a/1667114/1447225.
    set(CMAKE_C_FLAGS_DEBUG
        "${CMAKE_C_FLAGS_DEBUG}"
        -std=c99 -O0 -ggdb3 -pg --coverage
        -Wall
        -Wextra
        -Wpedantic
        -Wbad-function-cast
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wfloat-equal
        -Wformat=2
        -Winline
        -Wjump-misses-init
        -Wlogical-op
        # -Wmissing-declarations
        -Wmissing-field-initializers
        -Wmissing-include-dirs
        # -Wmissing-prototypes
        -Wnested-externs
        -Wold-style-definition
        -Woverlength-strings
        -Wpointer-arith
        -Wredundant-decls
        -Wshadow
        -Wstack-protector
        -Wstrict-aliasing=2
        -Wstrict-prototypes
        -Wswitch-default
        -Wundef
        -Wunreachable-code
        #-Wunsuffixed-float-constants
        -Wunused
        # -Wuseless-cast
        -Wwrite-strings
        -ffloat-store
        -fno-common
        -fstrict-aliasing
        -ftree-vrp)

    # Flattern options list properly.
    string(REPLACE ";" " " CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()

configure_file(
    "${S}/config.h.in"
    "${B}/include/rvlm/fdtd/config.h")

include_directories(
    "${S}/include"
    "${B}/include"
    "${NETCDF_INCLUDE_DIRS}")

add_library(rvlmfdtd STATIC
    "${S}/include/rvlm/fdtd/common/cross_compiler.h"
    "${S}/include/rvlm/fdtd/common/internal_macros.h"
    "${S}/include/rvlm/fdtd/common/string_format.h"
    "${S}/include/rvlm/fdtd/common/common_typedefs.h"
    "${S}/include/rvlm/fdtd/common/error_handling.h"
    "${S}/include/rvlm/fdtd/common/memory_allocation.h"
    "${S}/include/rvlm/fdtd/common/netcdf_helpers.h"
    "${S}/include/rvlm/fdtd/common/problem_formulation.h"
    "${S}/include/rvlm/fdtd/common/problem_formulation_netcdf.h"
    "${S}/src/common/string_format.c"
    "${S}/src/common/error_handling.c"
    "${S}/src/common/memory_allocation.c"
    "${S}/src/common/netcdf_helpers.c"
    "${S}/src/common/problem_formulation.c"
    "${S}/src/common/problem_formulation_netcdf.c"
    "${S}/config.h.in")

target_link_libraries(rvlmfdtd ${NETCDF_LIBRARIES})

# -- unit tests --
if(RFDTD_CONFIG_UNIT_TESTS)
    find_package(PythonInterp REQUIRED)
    find_package(CMocka       REQUIRED)
    find_program(NCGEN_EXECUTABLE ncgen)
    enable_testing()

    configure_file(
        "${S}/test/test_config.h.in"
        "${B}/include/rvlm/fdtd/test_config.h")

    include_directories(
        "${S}/include"
        "${B}/include"
        "${CMOCKA_INCLUDE_DIR}")

    add_executable(rvlmfdtd-test
        "${B}/include/generated-tests.h"
        "${B}/test/_data/yee_lattice_1x2x2.nc"
        "${S}/generate_test_registry.py"
	    "${S}/test/common/test_string_format.c"
	    "${S}/test/common/test_error_handling.c"
        "${S}/test/common/test_internal_macros.c"
        "${S}/test/common/test_memory_allocation.c"
        "${S}/test/common/test_problem_formulation_netcdf.c"
        "${S}/test/main_cmocka.c"
        "${S}/test/test_config.h.in")

    add_custom_command(
        OUTPUT  "${B}/test/_data/yee_lattice_1x2x2.nc"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${B}/test/_data"
        COMMAND "${NCGEN_EXECUTABLE}"
                    -o "${B}/test/_data/yee_lattice_1x2x3.nc"
                       "${S}/test/_data/yee_lattice_1x2x3.ncdl"
        COMMENT "Generating NetCDF files for unit tests")

    # Gather all '*.c' files from 'rvlmfdtd-test' target into TEST_SOURCES.
    get_target_property(TEST_SOURCES_TMP rvlmfdtd-test SOURCES)
    set(TEST_SOURCES)
    foreach(ITEM ${TEST_SOURCES_TMP})
        string(REGEX MATCH ".c$" ITEM_MATCH "${ITEM}")
        if(ITEM_MATCH)
            list(APPEND TEST_SOURCES "${ITEM}")
        endif()
    endforeach()

    add_custom_command(
        OUTPUT  "${B}/include/generated-tests.h"
        DEPENDS "${S}/generate_test_registry.py" ${TEST_SOURCES}
        COMMAND "${PYTHON_EXECUTABLE}"
                "${S}/generate_test_registry.py"
                    --output "${B}/include/generated-tests.h"
                              ${TEST_SOURCES}
        COMMENT "Generating CUnit registry header file")

    target_link_libraries(rvlmfdtd-test rvlmfdtd "${CMOCKA_LIBRARY}")

    if (RFDTD_CONFIG_VALGRIND)
    add_test(rvlmfdtd-test valgrind ./rvlmfdtd-test)
    else()
    add_test(rvlmfdtd-test rvlmfdtd-test)
    endif()
endif()

# -- cppcheck --
if(RFDTD_CONFIG_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    add_custom_target(cppcheck ALL
        "${CPPCHECK}" --quiet
                      --template=gcc
                      --language=c
                      --std=c99
                      --enable=all
                      -I "${S}/include"
                      "${S}/src"
        DEPENDS rvlmfdtd
        WORKING_DIRECTORY "${S}"
        COMMENT "Analyzing sources with cppcheck")
endif()

# -- installation --
# This library ca be installed under any prefix, so do not use absolute paths
# for destinations. Source paths must be absolute anyway.
install(
    TARGETS rvlmfdtd
    ARCHIVE DESTINATION "lib")
install(
    FILES "${B}/include/rvlm/fdtd/config.h"
    DESTINATION "include/rvlm/fdtd")
install(
    DIRECTORY "${S}/include/" # NB: trailing slash!
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h")

if(RFDTD_INSTALL_SOURCES)
    install(
        DIRECTORY "${S}/src/"
        DESTINATION "src/rvlm-libfdtd"
        FILES_MATCHING PATTERN "*.c")
endif()
