add_executable(test.serializer test_serializer.cpp)
target_link_libraries(test.serializer heterogeneous_tree visitor_impl GTest::gtest)
add_test(NAME test.serializer
        COMMAND test.serializer)

add_executable(test.print test_print.cpp)
target_link_libraries(test.print heterogeneous_tree visitor_impl GTest::gtest)
add_test(NAME test.print
        COMMAND test.print)