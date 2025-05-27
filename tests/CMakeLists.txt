add_executable(dbasic_test_dud "")
target_link_directories(dbasic_test_dud PRIVATE "${PROJ_LIBS_DIR}")
target_include_directories(dbasic_test_dud INTERFACE "${PROJ_INCLUDE_DIR}")
target_sources(dbasic_test_dud PRIVATE dbasic_test_dud.c)
