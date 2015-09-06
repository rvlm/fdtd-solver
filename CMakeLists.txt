cmake_minimum_required(VERSION 2.8)
project(rFDTD C)

# Just shortcuts. They're short, nice and distinctive, so any path must be
# explicitly specified as for source directory or build directory (with some
# small exceptions like for 'install' command).
set(S "${CMAKE_CURRENT_SOURCE_DIR}")
set(B "${CMAKE_CURRENT_BINARY_DIR}")

# Build options.
option(RFDTD_CONFIG_USE_DOUBLE "Use double precision floats")
option(RFDTD_CONFIG_UNIT_TESTS "Build unit-tests for library")
option(RFDTD_CONFIG_CPPCHECK   "Check library sources with CppCheck")
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
    "${B}/include")

add_library(rvlmfdtd STATIC
    "${S}/include/rvlm/fdtd/common/common_helpers_internal.h"
    "${S}/include/rvlm/fdtd/common/common_helpers.h"
    "${S}/include/rvlm/fdtd/common/common_typedefs.h"
    "${S}/include/rvlm/fdtd/common/error_handling.h"
    "${S}/include/rvlm/fdtd/common/memory_allocation.h"    
    "${S}/include/rvlm/fdtd/common/simulation_parameters_enums.h"
    "${S}/include/rvlm/fdtd/common/simulation_parameters_reader.h"
    "${S}/include/rvlm/fdtd/common/simulation_parameters.h"
    "${S}/src/common/common_helpers.c"
    "${S}/src/common/error_handling.c"
    "${S}/src/common/memory_allocation.c"
    "${S}/src/common/simulation_parameters_reader.c"
    "${S}/config.h.in")

# -- unit tests --
if(RFDTD_CONFIG_UNIT_TESTS)
    find_program(PYTHON python REQUIRED)
    find_library(CUNIT_LIBRARIES cunit REQUIRED)
    enable_testing()

    add_executable(rvlmfdtd-test
        "${B}/include/generated-tests.h"
        "${S}/generate-test-registry.py"
	"${S}/test/common/test_common_helpers.c"
	"${S}/test/common/test_error_handling.c"
        "${S}/test/main.c")

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
        DEPENDS "${S}/generate-test-registry.py" ${TEST_SOURCES}
        COMMAND "${PYTHON}"
                "${S}/generate-test-registry.py"
                    --output "${B}/include/generated-tests.h"
                              ${TEST_SOURCES}
        COMMENT "Generating CUnit registry header file")

    target_link_libraries(rvlmfdtd-test rvlmfdtd ${CUNIT_LIBRARIES})
    add_test(rvlmfdtd-test rvlmfdtd-test)
endif()

# -- cppcheck --
if(RFDTD_CONFIG_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    add_custom_target(cppcheck ALL
        "${CPPCHECK}" --quiet
                      --template=gcc
                      --language=c
                      --std=c89
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
