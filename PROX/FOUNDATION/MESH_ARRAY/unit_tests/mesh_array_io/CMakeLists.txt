CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/unit_tests/mesh_array_io/mesh_array_data_path.h.in"
  "${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/unit_tests/mesh_array_io/mesh_array_data_path.h"
  @ONLY
  )

INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_io 
  mesh_array_io.cpp
  mesh_array_data_path.h
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_io
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mesh_array
  tiny
  )

ADD_TEST( 
  unit_mesh_array_io
  unit_mesh_array_io
  )

