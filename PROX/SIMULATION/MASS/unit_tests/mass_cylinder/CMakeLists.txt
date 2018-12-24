INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_cylinder
  mass_cylinder.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_cylinder
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mass
)

ADD_TEST(
  unit_mass_cylinder
  unit_mass_cylinder
  )

