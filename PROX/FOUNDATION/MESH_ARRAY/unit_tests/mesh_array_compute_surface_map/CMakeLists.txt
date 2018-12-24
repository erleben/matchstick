INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_mesh_array_compute_surface_map
  mesh_array_compute_surface_map.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_compute_surface_map
  mesh_array
  tiny
  tetgen
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_mesh_array_compute_surface_map
  unit_mesh_array_compute_surface_map
  )

