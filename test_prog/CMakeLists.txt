add_executable(test_prog test.c)

target_link_libraries(test_prog p1_playground)

add_test(NAME test_prog
        COMMAND test_prog)