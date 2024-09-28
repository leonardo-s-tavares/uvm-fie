################################################################################
# This is the CMakeLists file for the uvmfi library directory.
#
# Its main goals are:
#   - Fetch library files.
#   - Creates target.
#       `- Add uvm target library.
#       `- Set target_link_libraries.
# 	`- Set target_include_directories.
#
################################################################################


################################################################################
# Fetch files
################################################################################

# Fetch .cpp files for library creation
file(GLOB_RECURSE uvmfi_source
    "*.cpp"
)

# Fetch .h files for library creation
file(GLOB_RECURSE uvmfi_headers
    "*.h"
)

################################################################################
# Library target
################################################################################

# Create a library called "uvmfi" which includes the source files uvmfi_source.
add_library(uvmfi ${uvmfi_source})

# Link the target library to systemc and uvm libraries.
target_link_libraries(uvmfi SystemCLanguage::systemc UVM::uvm)

# Make sure the compiler can find include files for uvmfi library
# when other libraries or executables link to uvmfi
target_include_directories(uvmfi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)


################################################################################
# Set up installation 
################################################################################

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
)

install(FILES ${uvmfi_headers}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS uvmfi
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT ${PROJECT_NAME}-targets
)
