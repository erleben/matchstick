INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/MESH_ARRAY/MESH_ARRAY/include
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mesh_array_tetrahedron_attribute
  mesh_array_tetrahedron_attribute.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mesh_array_tetrahedron_attribute
  mesh_array
  tiny
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_mesh_array_tetrahedron_attribute
  unit_mesh_array_tetrahedron_attribute
  )

