set(PROJ_SOURCES
    ${PROJ_SOURCE_DIR}/cstrview.c
    ${PROJ_SOURCE_DIR}/safestr.c
    ${PROJ_SOURCE_DIR}/dict.c
    ${PROJ_SOURCE_DIR}/files.c
    ${PROJ_SOURCE_DIR}/token.c
    ${PROJ_SOURCE_DIR}/lexer.c
)

add_library(dbasic "")
target_include_directories(dbasic PUBLIC "${PROJ_INCLUDE_DIR}")
target_sources(dbasic PRIVATE ${PROJ_SOURCES})

add_executable(derkbasic "")
target_link_directories(derkbasic PRIVATE "${PROJ_LIBS_DIR}")
target_include_directories(derkbasic PUBLIC "${PROJ_INCLUDE_DIR}")
# target_link_libraries(derkbasic PRIVATE dbasic) # please uncomment when all unit tests pass!
target_sources(derkbasic PRIVATE main.c)
