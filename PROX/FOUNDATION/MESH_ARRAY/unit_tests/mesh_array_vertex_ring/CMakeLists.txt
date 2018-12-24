INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_vertex_ring 
  mesh_array_vertex_ring.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_vertex_ring
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mesh_array
  tiny
  )

ADD_TEST( 
  unit_mesh_array_vertex_ring
  unit_mesh_array_vertex_ring
  )

