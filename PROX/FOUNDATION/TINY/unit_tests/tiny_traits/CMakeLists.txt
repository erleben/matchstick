INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_traits
  tiny_traits.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_traits
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_traits
  unit_tiny_traits
  )

