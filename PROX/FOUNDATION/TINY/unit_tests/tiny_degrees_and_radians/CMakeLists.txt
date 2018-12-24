INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_degrees_and_radians
  tiny_degrees_and_radians.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_degrees_and_radians
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_degrees_and_radians
  unit_tiny_degrees_and_radians
  )
