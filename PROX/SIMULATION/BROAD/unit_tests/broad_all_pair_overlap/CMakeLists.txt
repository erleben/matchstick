INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/SIMULATION/BROAD/BROAD/include  
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include
${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/UTIL/UTIL/include
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_broad_all_pair_overlap
  broad_all_pair_overlap.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_broad_all_pair_overlap
  tiny
  util
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_broad_all_pair_overlap
  unit_broad_all_pair_overlap
  )


