#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SphericalProjection::SphericalProjection" for configuration "Debug"
set_property(TARGET SphericalProjection::SphericalProjection APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SphericalProjection::SphericalProjection PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "/home/hvh/MyGit/SphericalProjection/install/lib/libSphericalProjection.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SphericalProjection::SphericalProjection )
list(APPEND _IMPORT_CHECK_FILES_FOR_SphericalProjection::SphericalProjection "/home/hvh/MyGit/SphericalProjection/install/lib/libSphericalProjection.a" )

# Import target "SphericalProjection::CSVReader" for configuration "Debug"
set_property(TARGET SphericalProjection::CSVReader APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SphericalProjection::CSVReader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "/home/hvh/MyGit/SphericalProjection/install/lib/libCSVReader.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SphericalProjection::CSVReader )
list(APPEND _IMPORT_CHECK_FILES_FOR_SphericalProjection::CSVReader "/home/hvh/MyGit/SphericalProjection/install/lib/libCSVReader.a" )

# Import target "SphericalProjection::main" for configuration "Debug"
set_property(TARGET SphericalProjection::main APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SphericalProjection::main PROPERTIES
  IMPORTED_LOCATION_DEBUG "/home/hvh/MyGit/SphericalProjection/install/bin/main"
  )

list(APPEND _IMPORT_CHECK_TARGETS SphericalProjection::main )
list(APPEND _IMPORT_CHECK_FILES_FOR_SphericalProjection::main "/home/hvh/MyGit/SphericalProjection/install/bin/main" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
