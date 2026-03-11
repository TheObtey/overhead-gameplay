project(Foundry)

find_package(unofficial-enet CONFIG REQUIRED)
find_package(ReactPhysics3D REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(raylib REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

if (MSVC)
    add_compile_options(/bigobj)
else ()
    add_compile_options(-Wa,-mbig-obj)
endif ()

add_library(${PROJECT_NAME} STATIC
    ${SOURCES})

add_library(Foundry::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${SOL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ReactPhysics3D::ReactPhysics3D)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::enet::enet)
target_link_libraries(${PROJECT_NAME} PUBLIC Ore::library)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::lua::lua)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

