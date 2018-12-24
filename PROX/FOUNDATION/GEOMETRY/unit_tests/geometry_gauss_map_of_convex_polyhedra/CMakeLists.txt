INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/GEOMETRY/GEOMETRY/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_geometry_gauss_map_of_convex_polyhedra
  geometry_gauss_map_of_convex_polyhedra.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_geometry_gauss_map_of_convex_polyhedra
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_geometry_gauss_map_of_convex_polyhedra
  unit_geometry_gauss_map_of_convex_polyhedra
  )

