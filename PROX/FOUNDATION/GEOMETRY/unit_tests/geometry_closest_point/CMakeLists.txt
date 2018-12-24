INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/GEOMETRY/GEOMETRY/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_geometry_closest_point
  geometry_closest_point.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_geometry_closest_point
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_geometry_closest_point
  unit_geometry_closest_point
  )

