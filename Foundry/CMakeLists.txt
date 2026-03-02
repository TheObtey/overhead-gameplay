project(Foundry)

find_package(unofficial-enet CONFIG REQUIRED)
find_package(ReactPhysics3D REQUIRED)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)

add_library(Foundry::library ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE ReactPhysics3D::ReactPhysics3D)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::enet::enet)
target_link_libraries(${PROJECT_NAME} PRIVATE Ore::library)

