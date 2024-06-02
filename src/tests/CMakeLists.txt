set(TEST_FILES 
    getTest.cpp 
    putTest.cpp
    hashTest.cpp
    sizeTest.cpp
    containTest.cpp
)

set(THIRD_LIBRARY
    fmt::fmt
    gtest
    gtest_main
)

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} ${THIRD_LIBRARY})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()