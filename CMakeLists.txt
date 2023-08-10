# ──────────────────────────────────────────────────────────────────────────── #
# Project                                                                      #

cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(point-cloud-denoising LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})




# ──────────────────────────────────────────────────────────────────────────── #
# Flags                                                                        #
# Define CMake flags related to C++, some compilation flags and library flags  #

# CMake flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread")
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

# Compilation flags
if(MSVC)
  # MSVC https://msdn.microsoft.com/en-us/library/thxezb7y.aspx
  add_compile_options(/MP /W4 /WX)
else()
  # Gcc   https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  # Clang http://clang.llvm.org/docs/UsersManual.html
  add_compile_options(-fdiagnostics-show-option -Wall -Wextra -pedantic -Werror)
  endif()

# Sanitize flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_package(Sanitizers)
endif()

# Ignore PCL errors in Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wno-gnu-anonymous-struct -Wno-nested-anon-types)
endif()





# ──────────────────────────────────────────────────────────────────────────── #
# Find packages #

# OpenCV
find_package(OpenCV CONFIG REQUIRED) # ${OpenCV_LIBRARIES}

# # TinyXML2
# find_package(TinyXML2 MODULE REQUIRED) # ${TinyXML2_LIBRARIES}





# ──────────────────────────────────────────────────────────────────────────── #
# Add targets #

add_subdirectory(point_cloud_denoising)





# ──────────────────────────────────────────────────────────────────────────── #
# Uninstallation #

if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
  )

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  )
endif()

