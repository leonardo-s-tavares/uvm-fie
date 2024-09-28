find_path(SystemCLanguage_INCLUDE_DIR "systemc.h" PATHS "/usr/local/systemc-2.3.3/include" )
find_library(SystemCLanguage_shared_LIBRARY "libsystemc.so" NAMES "libsystemc.so" PATHS "/usr/local/systemc-2.3.3/lib-linux64")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SystemCLanguage DEFAULT_MSG
    SystemCLanguage_INCLUDE_DIR
    SystemCLanguage_shared_LIBRARY
)

add_library(SystemCLanguage::systemc SHARED IMPORTED)
set_target_properties(SystemCLanguage::systemc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${SystemCLanguage_INCLUDE_DIR}
    IMPORTED_LOCATION ${SystemCLanguage_shared_LIBRARY}
)
