# Install script for directory: /home/hvh/MyGit/SphericalProjection

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/hvh/MyGit/SphericalProjection/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake"
         "/home/hvh/MyGit/SphericalProjection/build/CMakeFiles/Export/_home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/lib/cmake" TYPE FILE FILES "/home/hvh/MyGit/SphericalProjection/build/CMakeFiles/Export/_home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig-debug.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/lib/cmake" TYPE FILE FILES "/home/hvh/MyGit/SphericalProjection/build/CMakeFiles/Export/_home/hvh/MyGit/SphericalProjection/install/lib/cmake/SphericalProjectionConfig-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hvh/MyGit/SphericalProjection/install/lib/libSphericalProjection.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/lib" TYPE STATIC_LIBRARY FILES "/home/hvh/MyGit/SphericalProjection/build/libSphericalProjection.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hvh/MyGit/SphericalProjection/install/lib/libCSVReader.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/lib" TYPE STATIC_LIBRARY FILES "/home/hvh/MyGit/SphericalProjection/build/libCSVReader.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main"
         RPATH "/home/hvh/MyGit/SphericalProjection/install/lib")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hvh/MyGit/SphericalProjection/install/bin/main")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/bin" TYPE EXECUTABLE FILES "/home/hvh/MyGit/SphericalProjection/build/main")
  if(EXISTS "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main"
         OLD_RPATH "/home/hvh/Libraries/OpenCV_4_5_2/install_with_GTK/lib:"
         NEW_RPATH "/home/hvh/MyGit/SphericalProjection/install/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/strip" "$ENV{DESTDIR}/home/hvh/MyGit/SphericalProjection/install/bin/main")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hvh/MyGit/SphericalProjection/install/include/CSVReader.h;/home/hvh/MyGit/SphericalProjection/install/include/SphericalProjection.h;/home/hvh/MyGit/SphericalProjection/install/include/rapidcsv.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hvh/MyGit/SphericalProjection/install/include" TYPE FILE FILES
    "/home/hvh/MyGit/SphericalProjection/include/CSVReader.h"
    "/home/hvh/MyGit/SphericalProjection/include/SphericalProjection.h"
    "/home/hvh/MyGit/SphericalProjection/include/rapidcsv.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/hvh/MyGit/SphericalProjection/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
