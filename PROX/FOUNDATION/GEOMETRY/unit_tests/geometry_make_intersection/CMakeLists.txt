INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/GEOMETRY/GEOMETRY/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_geometry_make_intersection
  geometry_make_intersection.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_geometry_make_intersection
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_geometry_make_intersection
  unit_geometry_make_intersection
  )

