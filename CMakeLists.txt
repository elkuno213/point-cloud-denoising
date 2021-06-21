##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (PointCloud_Denoising VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 17)

include               (set_max_warning_level)
set_max_warning_level ()


##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install CACHE PATH "Prefix added to install directories")


##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES src/*.c src/*.cpp)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC})

include            (assign_source_group)
assign_source_group(${PROJECT_FILES})


##################################################  Dependencies  ##################################################
include(import_library)

# ADD LIBRARIES HERE.
# Examples:
# - Header Only:
#   find_package  (GLM REQUIRED)
#   import_library(GLM_INCLUDE_DIRS)
# - Identical Debug and Release:
#   find_package  (OpenGL REQUIRED)
#   import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
# - Separate Debug and Release:
#   find_package  (Boost REQUIRED iostreams)
#   import_library(Boost_INCLUDE_DIRS Boost_IOSTREAMS_LIBRARY_DEBUG Boost_IOSTREAMS_LIBRARY_RELEASE)
# - You may also set the PROJECT_INCLUDE_DIRS and PROJECT_LIBRARIES instead of using import_library.

find_package(OpenCV REQUIRED PATHS /home/hvh/Libraries/lib/OpenCV_4_5_2_with_GTK/lib/cmake/opencv4) # OpenCV libraries
find_package(ParameterFinder REQUIRED PATHS /home/hvh/Libraries/lib/ParameterFinder/cmake) # ParameterFinder library
find_package(tinyxml2 REQUIRED PATHS /home/hvh/Libraries/lib/TinyXML2_8_0_0/lib/cmake/tinyxml2) # TinyXML2 library

# In case that the 3rdparty is out of the source tree
# set(PROJECT_INCLUDE_DIRS  ${PROJECT_INCLUDE_DIRS}
#                           ${OpenCV_INCLUDE_DIRS}
# )

set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} 
                      ${OpenCV_LIBS} 
                      tinyxml2::tinyxml2
                      ParameterFinder
)

# message(PROJECT_INCLUDE_DIRS="${PROJECT_INCLUDE_DIRS}")
# message(PROJECT_LIBRARIES="${PROJECT_LIBRARIES}")


##################################################    Targets     ##################################################
add_executable(${PROJECT_NAME} ${PROJECT_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()


##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing     ()
  file(GLOB_RECURSE PROJECT_TEST_CPPS tests/*.cpp)
  foreach(_SOURCE ${PROJECT_TEST_CPPS})
    get_filename_component    (_NAME ${_SOURCE} NAME_WE)
    add_executable            (${_NAME} ${_SOURCE})
    target_include_directories(${_NAME} PUBLIC 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<INSTALL_INTERFACE:include> PRIVATE src)
    target_include_directories(${_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
    target_link_libraries     (${_NAME} PUBLIC ${PROJECT_LIBRARIES})
    target_compile_definitions(${_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
    add_test                  (${_NAME} ${_NAME})
    set_property              (TARGET ${_NAME} PROPERTY FOLDER tests)
    assign_source_group       (${_SOURCE})
  endforeach()
endif()


##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}Config DESTINATION cmake)
export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}Config.cmake)


#################################################  Uninstallation  #################################################
if(NOT TARGET uninstall)
  configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

  add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)
endif()


################################################  Dependencies graph  ###############################################
find_program(DOT_EXE "dot")
if(DOT_EXE)
    message(STATUS "dot found: ${DOT_EXE}")
else()
    message(STATUS "dot not found!")
endif()

set(DOT_OUTPUT_TYPE "png" CACHE STRING "Build a dependency graph. Options are dot output types: ps, png, pdf..." )

if(DOT_EXE)
  add_custom_target(dependency-graph ALL
      COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} --graphviz=${CMAKE_INSTALL_PREFIX}/graphviz/${PROJECT_NAME}.dot .
      COMMAND ${DOT_EXE} -T${DOT_OUTPUT_TYPE} ${CMAKE_INSTALL_PREFIX}/graphviz/${PROJECT_NAME}.dot -o ${CMAKE_INSTALL_PREFIX}/graphviz/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}
  )

  add_custom_command(
      TARGET dependency-graph POST_BUILD
      COMMAND ;
      COMMENT
      "Dependency graph generated and located at ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}"
  )
endif()

