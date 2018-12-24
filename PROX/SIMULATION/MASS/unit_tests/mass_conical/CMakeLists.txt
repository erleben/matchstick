INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_conical
  mass_conical.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_conical 
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mass
)

ADD_TEST(
  unit_mass_conical
  unit_mass_conical
  )

