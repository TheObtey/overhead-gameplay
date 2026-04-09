project(Editor)


find_package(imgui CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(ReactPhysics3D REQUIRED)

file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/Libs/*.cpp"
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCES})

add_library(Editor::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/Libs/
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Foundry::library
    ReactPhysics3D::ReactPhysics3D
)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

