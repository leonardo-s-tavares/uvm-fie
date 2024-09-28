################################################################################
# This is the CMakeLists file for include the dut and testbench directories.
#
# Its main goals are:
#   - Add subdirectories.
#
################################################################################


################################################################################
# Add subdirectories
################################################################################

# Add design under test subdirectory
add_subdirectory(dut)
# Add testbench subdirectory
add_subdirectory(testbench)


