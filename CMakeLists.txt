cmake_minimum_required(VERSION 3.0.0)
project(PointCloud_Denoising VERSION 1.0.0)


# ==============
# Default config
# ==============
include(GNUInstallDirs) # Make cache variables for install destinations
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_config.cmake.in) # Get desired config file


# ========================
# Specify the C++ standard
# ========================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)


# ==============
# Set build type
# ==============
set(DEFAULT_BUILD_TYPE "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# ==============================
# Link libraries via its modules
# ==============================
# Link OpenCV libraries
find_package(OpenCV REQUIRED PATHS /home/hvh/Libraries/OpenCV_4_5_2/install_with_GTK/lib/cmake/opencv4)


# ==================================================================
# Add source files, libraries and define headers for these libraries
# ==================================================================
add_library(SphericalProjection src/SphericalProjection.cpp)
target_link_libraries(SphericalProjection ${OpenCV_LIBS})

add_library(ImageDenoising src/ImageDenoising.cpp)
target_link_libraries(ImageDenoising ${OpenCV_LIBS})

add_library(CSVReader src/CSVReader.cpp)
target_link_libraries(CSVReader)

add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS} SphericalProjection CSVReader ImageDenoising)

add_executable(denoising src/denoising.cpp)
target_link_libraries(denoising ${OpenCV_LIBS} ImageDenoising)

# Include header files
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
)

# ===================================================
# Export config file to the project's build directory
# ===================================================
get_all_targets(PROJECT_TARGETS)
export( TARGETS ${PROJECT_TARGETS}
		NAMESPACE ${PROJECT_NAME}:: 
		FILE ${PROJECT_NAME}Config.cmake)


# =========================================
# Install config files to desired locations
# =========================================
install(EXPORT ${PROJECT_NAME}Config
		FILE ${PROJECT_NAME}Config.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
		# DESTINATION ${CMAKE_SOURCE_DIR}/install/cmake)


# ========================================
# Install targets to the desired locations
# ========================================
install(TARGETS ${PROJECT_TARGETS} EXPORT ${PROJECT_NAME}Config
ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# =============================================
# Install header files to the desired locations
# =============================================
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h") # Get a list of header files
install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# =========
# Uninstall
# =========
if(NOT TARGET uninstall)
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
		IMMEDIATE @ONLY)

	add_custom_target(uninstall
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)
endif()


# ==================
# Dependencies graph
# ==================
find_program(DOT_EXE "dot")
if(DOT_EXE)
    message(STATUS "dot found: ${DOT_EXE}")
else()
    message(STATUS "dot not found!")
endif()

set(DOT_OUTPUT_TYPE "png" CACHE STRING "Build a dependency graph. Options are dot output types: ps, png, pdf..." )

if(DOT_EXE)
    add_custom_target(dependency-graph ALL
        COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} --graphviz=${CMAKE_INSTALL_DIR}/graphviz/${PROJECT_NAME}.dot .
        COMMAND ${DOT_EXE} -T${DOT_OUTPUT_TYPE} ${CMAKE_INSTALL_DIR}/graphviz/${PROJECT_NAME}.dot -o ${CMAKE_INSTALL_DIR}/graphviz/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}
    )

    add_custom_command(
        TARGET dependency-graph POST_BUILD
        COMMAND ;
        COMMENT
        "Dependency graph generated and located at ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}"
    )
endif()