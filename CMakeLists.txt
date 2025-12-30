cmake_minimum_required(VERSION 3.20)
# enable_testing()

## Project
project(ember
	VERSION 0.4.1
	DESCRIPTION "The EMBER Framework"
	LANGUAGES CXX
	)

# CMake Options
# option(DEBUG           "Add debug build options to all the targets"                              OFF)
option(BUILD_EXAMPLES  "Build EMBER examples"                                                    OFF)
option(BUILD_TESTS     "Generate build files for unit tests"                                     OFF)
option(BUILD_VERILATOR "Compile verilator submodules (requires verilator version >= 4.0; < 5.0)" OFF)
option(BUILD_SABOTEURS "Compile default saboteurs library"                                       ON)

# Target
add_library(ember STATIC)

# C++ Standard
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD 20)
target_compile_features(ember PUBLIC cxx_std_20)

# Compile-Time Options
if((NOT DEFINED CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # target_compile_options(ember "-fmax-errors=10 -Wno-write-strings -Wno-sign-compare -O0 -ggdb -g -Wall -fanalyzer -pedantic -Werror -DDEBUG")
    target_compile_options(ember PRIVATE "-Wno-write-strings -Wno-sign-compare -O0 -ggdb -g -Wall -fanalyzer -DDEBUG")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(ember PRIVATE "-Wno-write-strings -Wno-sign-compare -O3")
endif()


## Subdirectories
add_subdirectory(ember)

## Build Extras
include(CTest)
if(BUILD_EXAMPLES OR BUILD_TESTS)
	enable_testing()
endif()

# Build Saboteurs Library
if(BUILD_SABOTEURS)
	add_subdirectory(saboteurs)
endif()

# Build Verilator Extension
if(BUILD_VERILATOR)
	add_subdirectory(verilator)
endif()

# Build Examples
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

### --- PACKAGING --- ###

# ---- Build-tree package files (no install required) ----
include(CMakePackageConfigHelpers)

# Export the wrapper target "core" so consumers get ember::core
export(
  TARGETS ember
  NAMESPACE ember::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/emberTargets.cmake"
)

# Generate emberConfig.cmake in the build dir
configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/emberConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/emberConfig.cmake"
  INSTALL_DESTINATION "."   # irrelevant for build-tree use, but required by the helper
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Optional version file (nice, but not required)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/emberConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)
