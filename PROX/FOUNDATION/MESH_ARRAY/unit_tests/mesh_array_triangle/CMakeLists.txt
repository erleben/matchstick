INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_triangle 
  mesh_array_triangle.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_triangle
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mesh_array
  tiny
  )

ADD_TEST( 
  unit_mesh_array_triangle
  unit_mesh_array_triangle
  )

