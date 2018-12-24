INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_contiguous_interval
  tiny_contiguous_interval.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_contiguous_interval
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_contiguous_interval
  unit_tiny_contiguous_interval
  )
