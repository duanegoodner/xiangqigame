
add_library(hash_calculator INTERFACE)
target_link_libraries(
        hash_calculator INTERFACE
        utility_functs
        game_board
        piece_position_points
        )
target_include_directories(hash_calculator INTERFACE ${BOOST_INCLUDEDIR})
message(STATUS "BOOST INCLUDE DIR = ${BOOST_INCLUDEDIR}")

add_executable(ks_test ks_test.cpp)
target_include_directories(ks_test PRIVATE ${BOOST_INCLUDEDIR})

