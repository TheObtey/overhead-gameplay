project(Foundry)

find_package(unofficial-enet CONFIG REQUIRED)
find_package(ReactPhysics3D REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

add_library(Foundry::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${SOL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ReactPhysics3D::ReactPhysics3D)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::enet::enet)
target_link_libraries(${PROJECT_NAME} PRIVATE Ore::library)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::lua::lua)

