INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_mesh 
  mesh_array_mesh.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_mesh
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mesh_array
  tiny
  )

ADD_TEST( 
  unit_mesh_array_mesh
  unit_mesh_array_mesh
  )

