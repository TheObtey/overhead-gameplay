project(Ore)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

add_library(Ore::library ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/../RHI/include/)

target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm-header-only)
target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

