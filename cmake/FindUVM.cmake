find_path(UVM_INCLUDE_DIR "uvm.h" PATHS "/usr/local/uvm-systemc/include" )
find_library(UVM_shared_LIBRARY "libuvm-systemc.so" NAMES "libuvm-systemc.so" PATHS "/usr/local/uvm-systemc/lib-linux64")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(UVM DEFAULT_MSG
    UVM_INCLUDE_DIR
    UVM_shared_LIBRARY
)

add_library(UVM::uvm SHARED IMPORTED)
set_target_properties(UVM::uvm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${UVM_INCLUDE_DIR}
    IMPORTED_LOCATION ${UVM_shared_LIBRARY}
)
