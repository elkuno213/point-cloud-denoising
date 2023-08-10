# ──────────────────────────────────────────────────────────────────────────── #
# Project #

cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(point-cloud-denoising LANGUAGES CXX VERSION 1.0.0)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)




# ──────────────────────────────────────────────────────────────────────────── #
# Add library and executables (binaries, examples, tests) #

set(point_cloud_denoising_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app)




# ──────────────────────────────────────────────────────────────────────────── #
# Installation #

# Install non-ROS targets
install(
  TARGETS point_cloud_denoising
          ${point_cloud_denoising_BINS}
  EXPORT  point_cloud_denoisingTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install only the header files for the library
install(
  DIRECTORY       ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION     ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING  PATTERN "*.h"
                  PATTERN "*.hpp"
                  PATTERN ".svn"  EXCLUDE
)

# Export targets CMake file in build tree
export(
  EXPORT point_cloud_denoisingTargets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/point_cloud_denoisingTargets.cmake
)

# Export target CMake file in installation tree
install(
  EXPORT point_cloud_denoisingTargets
  FILE point_cloud_denoisingTargets.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/point_cloud_denoising/cmake
)

# Generate package version file
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/point_cloud_denoisingConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Generate package configuration file
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/point_cloud_denoisingConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/point_cloud_denoisingConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/point_cloud_denoising/cmake
  PATH_VARS INCLUDE_INSTALL_DIR
)

# Install the package configuration files
install(
  FILES       ${CMAKE_CURRENT_BINARY_DIR}/point_cloud_denoisingConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/point_cloud_denoisingConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/point_cloud_denoising/cmake
)
