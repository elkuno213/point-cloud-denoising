# TODO: research why undefined symbol at runtime of application when using shared library
add_library(point_cloud_denoising STATIC)
target_sources(point_cloud_denoising PRIVATE
  CSVReader.hpp             CSVReader.cpp
  ImageDenoising.hpp        ImageDenoising.cpp
  SphericalProjection.hpp   SphericalProjection.cpp
  rapidcsv.h
)
target_include_directories(point_cloud_denoising PUBLIC
  $<BUILD_INTERFACE:${point_cloud_denoising_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(point_cloud_denoising PUBLIC
  ${OpenCV_INCLUDE_DIR}
)
target_link_libraries(point_cloud_denoising PUBLIC
  ${OpenCV_LIBRARIES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # add_sanitizers(point_cloud_denoising)
  # target_compile_definitions(point_cloud_denoising PUBLIC -DDEBUG_INFO)
endif()
