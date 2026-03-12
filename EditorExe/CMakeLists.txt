project(EditorExe)

#file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(MAINEDITOR "main" CACHE STRING "Select the program entry point")

set(SOURCE
    src/${MAINEDITOR}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE} )

#add_library(${PROJECT_NAME} SHARED
#    ${SOURCES}
#)
#
#add_library(testLib::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(${PROJECT_NAME} PRIVATE Editor::library)

