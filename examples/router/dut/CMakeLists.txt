################################################################################
# This is the CMakeLists file for dut library generation.
#
# Its main goals are:
#   - Fetch library files.
#   - Create dut library.
#       `- Creates the application executable.
#       `- Set target_link_libraries.
#
################################################################################


################################################################################
# Fetch files
################################################################################

# Fetch .cpp files for library creation
#file(GLOB dut_sources
#    "*/*.cpp"
#)

# Fetch .h files for library creation
file(GLOB dut_headers
    "*/*.h"
)


################################################################################
# Library target
################################################################################

# Create a library called "dut" which includes the source files dut_sources.
#add_library(dut_router ${dut_sources})

# Link the target library to systemc library.
#target_link_libraries(dut_router SystemCLanguage::systemc)

# Make sure the compiler can find include files for our dut library
# when other libraries or executables link to dut
#target_include_directories(dut_router PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#    $<INSTALL_INTERFACE:include>
#)

