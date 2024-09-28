# FIRST TIME:
cd <path_to>/uvm-fie
sudo docker build . --no-cache

# AFTER FIRST TIME, DO IT EVERY TIME TO RUN THE ROUTER'S SIMULATION:
sudo docker run -it -v "$PWD":/home/uvm-fi <docker_id> zsh
cd home/uvm-fi
./build_uvmfi_library.sh
export SC_SIGNAL_WRITE_CHECK=DISABLE
./build/examples/router/testbench/router
