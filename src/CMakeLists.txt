set(PROJ_SOURCES
    ${PROJ_SOURCE_DIR}/cstrview.c
)

add_library(dbasic "")
target_include_directories(dbasic PUBLIC "${PROJ_INCLUDE_DIR}")
target_sources(dbasic PRIVATE ${PROJ_SOURCES})

add_executable(derkbasic "")
target_link_directories(derkbasic PRIVATE "${PROJ_LIBS_DIR}")
target_include_directories(derkbasic PUBLIC "${PROJ_INCLUDE_DIR}")
target_link_libraries(derkbasic PRIVATE dbasic)
target_sources(derkbasic PRIVATE main.c)
