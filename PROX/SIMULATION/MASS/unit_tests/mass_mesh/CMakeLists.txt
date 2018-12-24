INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_mesh
  mass_mesh.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_mesh
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mass
)

ADD_TEST(
  unit_mass_mesh
  unit_mass_mesh
  )

