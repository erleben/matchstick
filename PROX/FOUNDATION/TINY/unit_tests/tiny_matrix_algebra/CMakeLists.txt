INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_matrix_algebra
  tiny_matrix_algebra.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_matrix_algebra
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_matrix_algebra
  unit_tiny_matrix_algebra
  )

