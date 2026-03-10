include(FetchContent)
project(Editor)

FetchContent_Declare(
  rlimgui
  GIT_REPOSITORY https://github.com/v2lmmj04/rlImGui.git
  GIT_TAG        origin/main
)
FetchContent_MakeAvailable(rlimgui)

FetchContent_Declare(
  rlgizmo
  GIT_REPOSITORY https://github.com/cloudofoz/raylib-gizmo.git
  GIT_TAG        origin/main
)

FetchContent_MakeAvailable(rlgizmo)

find_package(imgui CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_BINARY_DIR}/../_deps/rlimgui-src/*.cpp" "${PROJECT_BINARY_DIR}/../_deps/rlgizmo-src/src/*.c")

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

add_library(Editor::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_BINARY_DIR}/../_deps/rlimgui-src/
        ${PROJECT_BINARY_DIR}/../_deps/rlgizmo-src/src/
)

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

