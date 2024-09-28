################################################################################
# This is the CMakeLists file for example generation testbench executable.
#
# Its main goals are:
#   - Fetch library files.
#   - Create executable.
#       `- Creates the application executable.
#       `- Set target_link_libraries.
#
################################################################################


################################################################################
# Fetch files
################################################################################

# Fetch .cpp files for library creation
file(GLOB_RECURSE testbench_sources
    "*.cpp"
)

# Fetch .h files for library creation
file(GLOB_RECURSE testbench_headers
    "*.h"
)

################################################################################
# TestBench logic
################################################################################

# Add executable called "router" that is built from the source files
add_executable(router ${testbench_sources})

# Link the executable to the dut, systemc, uvm and uvmfi libraries.
target_link_libraries(router SystemCLanguage::systemc UVM::uvm uvmfi)

