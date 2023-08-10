option(WITH_BINS "Enable compiling and installing point_cloud_denoising binaries" ON)
set(point_cloud_denoising_BINS "")

if(WITH_BINS)
  message(STATUS "WITH_BINS=${WITH_BINS}")

  add_executable(point_cloud_denoising_bin
    point_cloud_denoising_bin.cpp
  )
  target_link_libraries(point_cloud_denoising_bin PRIVATE
    point_cloud_denoising
    pugixml
  )
  target_include_directories(point_cloud_denoising_bin PRIVATE
  )

  set(point_cloud_denoising_BINS
    ${point_cloud_denoising_BINS}
    point_cloud_denoising_bin
  )
else()
  message(STATUS "point_cloud_denoising' binaries are not compiled and installed")
endif()

set(point_cloud_denoising_BINS ${point_cloud_denoising_BINS} CACHE INTERNAL "List of point_cloud_denoising' binaries")
