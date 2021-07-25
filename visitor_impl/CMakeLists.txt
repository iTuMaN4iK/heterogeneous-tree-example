add_library(visitor_impl STATIC visitor_impl.cpp)

target_include_directories(visitor_impl
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

target_link_libraries(visitor_impl PUBLIC heterogeneous_tree)

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(tests)
endif ()