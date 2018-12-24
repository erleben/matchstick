INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_quaternion_algebra
  tiny_quaternion_algebra.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_quaternion_algebra
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_quaternion_algebra
  unit_tiny_quaternion_algebra
  )

