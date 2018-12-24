INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/TINY/TINY/include 
  ${Boost_INCLUDE_DIRS}
  )

ADD_EXECUTABLE(
  unit_tiny_coordsys_prod
  tiny_coordsys_prod.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_tiny_coordsys_prod
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_tiny_coordsys_prod
  unit_tiny_coordsys_prod
  )
