INCLUDE_DIRECTORIES( 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/SPARSE/SPARSE/include 
  ${PROJECT_SOURCE_DIR}/PROX/FOUNDATION/UTIL/UTIL/include 
  ${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE(
  unit_sparse_io
  sparse_io.cpp
  )

TARGET_LINK_LIBRARIES(
  unit_sparse_io
  util
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

ADD_TEST(
  unit_sparse_io
  unit_sparse_io
  )
