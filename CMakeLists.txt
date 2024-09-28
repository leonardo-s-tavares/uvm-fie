################################################################################
# This is the main CMakeLists file for the uvmfi library.
#
# Its main goals are:
#   - Set up global variables.
#   - Configure uvmfi dependencies (find_packages).
#   - Add subdirectories.
#
################################################################################


################################################################################
# Set up global variables
################################################################################

# Set up minimal cmake version
cmake_minimum_required(VERSION 3.5.1)

# Set project version
project(LibUVMfi VERSION 0.0.1)

# Set c++11 globally (!not sure about systemc compiled version)
#set(CMAKE_CXX_STANDARD 11)

# Set uvmfi source dir
set(uvmfi_source_dir ${PROJECT_SOURCE_DIR})

# NEW: To enable debug with GDB
set(CMAKE_BUILD_TYPE Debug)

# Override default value for the signal writer policy
# This is a run-time alternative to SC_DEFAULT_WRITER_POLICY
# Important to enable the intrusive mechnism of uvmfi
# option(SC_SIGNAL_WRITE_CHECK DISABLE) # NEW: Doesn't work

################################################################################
# Handle requirements
################################################################################

# Set Cmake module path for find_package
list(APPEND CMAKE_MODULE_PATH "${uvmfi_source_dir}/cmake")

# Find SystemC language
find_package(SystemCLanguage REQUIRED)

# Find UVM 
find_package(UVM REQUIRED)


################################################################################
# Project logic
################################################################################

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)

