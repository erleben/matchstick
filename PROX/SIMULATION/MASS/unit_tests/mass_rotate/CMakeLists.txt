INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/MASS/MASS/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_mass_rotate
  mass_rotate.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_mass_rotate
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  tiny
  mass
)

ADD_TEST(
  unit_mass_rotate
  unit_mass_rotate
  )

