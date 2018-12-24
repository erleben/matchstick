INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/GEOMETRY/GEOMETRY/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_geometry_obb
  geometry_obb.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_geometry_obb
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_geometry_obb
  unit_geometry_obb
  )

