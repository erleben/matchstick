INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_basic_math_types
  tiny_basic_math_types.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_basic_math_types
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_basic_math_types
  unit_tiny_basic_math_types
  )
