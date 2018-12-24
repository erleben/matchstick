INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_tetrahedron
  mass_tetrahedron.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_tetrahedron
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mass
)

ADD_TEST(
  unit_mass_tetrahedron
  unit_mass_tetrahedron
  )

