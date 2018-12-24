INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_circulator 
  mesh_array_circulator.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_circulator
  mesh_array
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST( 
  unit_mesh_array_circulator
  unit_mesh_array_circulator
  )

