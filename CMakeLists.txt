IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0043 NEW) # QT needs this?
  CMAKE_POLICY(SET CMP0054 NEW)
ENDIF(COMMAND cmake_policy)

PROJECT(MATCHSTICK_PROX)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0 FATAL_ERROR)

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

IF(MSVC)  
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

SET(Boost_USE_STATIC_LIBS ON)
IF (MSVC)
  SET( BOOST_LIBRARYDIR "${Boost_INCLUDE_DIR}/lib")
ENDIF(MSVC)
FIND_PACKAGE(
  Boost
  COMPONENTS
  unit_test_framework
  system
  filesystem
  REQUIRED
  )

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)

# Setup paths for Qt this is a little ugly
SET( TMP_CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH} )
SET( CMAKE_PREFIX_PATH "/Users/kenny/Qt/5.3/clang_64/" ${TMP_CMAKE_PREFIX_PATH} )
SET( CMAKE_PREFIX_PATH "/Users/kennyerleben/Qt/5.3/clang_64/" ${TMP_CMAKE_PREFIX_PATH} )
FIND_PACKAGE(Qt5Widgets  REQUIRED)
FIND_PACKAGE(Qt5Core     REQUIRED)
FIND_PACKAGE(Qt5Gui      REQUIRED)
FIND_PACKAGE(Qt5OpenGL   REQUIRED)
SET( CMAKE_PREFIX_PATH ${TMP_CMAKE_PREFIX_PATH} )

# Tell TinyXML to use std::strings over char *
ADD_DEFINITIONS(-DTIXML_USE_STL)

SET(ENABLE_PROFILING     1 CACHE STRING "Set to 1 if profiling should be enabled and 0 otherwise"                )
IF(ENABLE_PROFILING)
  MESSAGE("Profiling is...........................ON")
  ADD_DEFINITIONS(-DUSE_PROFILING)
ELSE()
  MESSAGE("Profiling is...........................OFF")
ENDIF()

SET(ENABLE_UNIT_TESTS 1 CACHE STRING "Set to 1 if unit tests should be added to project files and 0 otherwise")
IF(ENABLE_UNIT_TESTS)
  MESSAGE("Unit-tests are.........................ON")
  ENABLE_TESTING()
ELSE()
  MESSAGE("Unit-tests are.........................OFF")
ENDIF()

CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/bin/setup_default_files.cfg.in"
  "${PROJECT_SOURCE_DIR}/bin/setup_default_files.cfg"
  @ONLY
)

ADD_SUBDIRECTORY(PROX)

