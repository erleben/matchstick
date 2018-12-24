INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_polar_decomposition
  tiny_polar_decomposition.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_polar_decomposition
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_polar_decomposition
  unit_tiny_polar_decomposition
  )
