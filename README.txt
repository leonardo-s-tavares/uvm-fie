# FIRST TIME:
cd <path_to>/uvm-fie
sudo docker build . --no-cache

# AFTER FIRST TIME, DO IT EVERY TIME TO RUN THE ROUTER'S SIMULATION:
sudo docker run -it -v "$PWD":/home/uvm-fi <docker_id> zsh
cd home/uvm-fi
./build_uvmfi_library.sh # To compile
export SC_SIGNAL_WRITE_CHECK=DISABLE # Mandatory workaround. Run one time before simulating.
./build/examples/router/testbench/router # To simulate _________________________________________________________________________________________________
# The SystemC Code Converter's executable is at examples/router/converter.out
# The main source code of the converter is at examples/router/converter.c
# To convert a SystemC DUT design, copy each source file to examples/router/converter and name it as "input.h".
# Then run the converter's executable and follow the instructions at the prompt.
# The converted file will have comments in the lines converted and will be at examples/router/converter/output.h
# Compare the original file and the modified one. If is all OK, replace the original by the new one in the DUT's design (recommend to keep a backup of the original).

# Note: This current version converts only sc_in, sc_bv and enum variables into compatible ones. _________________________________________________________________________________________________
# The random fault injector's source code is present at examples/router/testbench/vip_fault_env.h
# It is implemented in a method called inject_random_fault()
# You can copy the implementation to your own vip_fault_env.h or use this file or this entire testbench as base to create yours.
# Make sure to set the "VARS" constant with the number of variables that you'll want to inject faults.
# To use it, first you need to put the list of variables to inject faults by putting a file at this same directory, called "variables_to_inject_faults.txt".
# The file needs to have one variable per line, with each information of the variable separated by comma.
# First you put the signal that will trigger the fault injection, then the type of the target fault variable, then its name (the one visible by the testbench).
# If any questions, use the pre-existing file as example.
# To use the method, just call it at the build phase of the testbench component that you want.
