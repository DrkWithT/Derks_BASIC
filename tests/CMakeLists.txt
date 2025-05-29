add_executable(dbasic_test_lexer "")
target_link_directories(dbasic_test_lexer PRIVATE "${PROJ_LIBS_DIR}/src")
target_include_directories(dbasic_test_lexer PUBLIC "${PROJ_INCLUDE_DIR}")
target_sources(dbasic_test_lexer PRIVATE dbasic_test_lexer.c)
target_link_libraries(dbasic_test_lexer PRIVATE dbasic)

add_test(NAME test_lexer_0 COMMAND "$<TARGET_FILE:dbasic_test_lexer>" "${PROJ_TEST_CODE_DIR}/hello.dbasic")
add_test(NAME test_lexer_1 COMMAND "$<TARGET_FILE:dbasic_test_lexer>" "${PROJ_TEST_CODE_DIR}/math.dbasic")
