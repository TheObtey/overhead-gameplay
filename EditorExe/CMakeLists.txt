project(EditorExe)

set(MAINEDITOR "main" CACHE STRING "Select the program entry point")

set(SOURCE
    src/${MAINEDITOR}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(${PROJECT_NAME} PRIVATE Editor::library)

