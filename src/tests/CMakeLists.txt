set(TEST_FILES
    putAndGetTest.cpp
    hashTest.cpp
    sizeTest.cpp
    delTest.cpp
    containTest.cpp
)

set(THIRD_LIBRARY
    gtest
    gtest_main
)

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} ${THIRD_LIBRARY})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()