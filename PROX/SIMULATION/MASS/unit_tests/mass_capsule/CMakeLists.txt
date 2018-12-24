INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_capsule
  mass_capsule.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_capsule 
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  mass
)

ADD_TEST(
  unit_mass_capsule
  unit_mass_capsule
  )

