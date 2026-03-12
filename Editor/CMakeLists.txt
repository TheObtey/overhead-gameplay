include(FetchContent)
project(Editor)


FetchContent_Declare(
  rlimgui
  GIT_REPOSITORY https://github.com/v2lmmj04/rlImGui.git
  GIT_TAG        origin/main
)
FetchContent_MakeAvailable(rlimgui)

FetchContent_Declare(
  imgui-filebrowser
  GIT_REPOSITORY https://github.com/AirGuanZ/imgui-filebrowser.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(imgui-filebrowser)

find_package(imgui CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp" 
    "${PROJECT_BINARY_DIR}/../_deps/rlimgui-src/*.cpp"
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCES})

add_library(Editor::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_BINARY_DIR}/../_deps/rlimgui-src/
        ${PROJECT_BINARY_DIR}/../_deps/imgui-filebrowser-src/
)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

