project(Foundry)

find_package(unofficial-enet CONFIG REQUIRED)
find_package(ReactPhysics3D REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)
#find_package(raylib REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

#include(FetchContent)
#
#FetchContent_Declare(
#    assimp
#    GIT_REPOSITORY https://github.com/assimp/assimp.git
#    GIT_TAG v5.3.1
#    GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(assimp)


file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES})

add_library(Foundry::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
        ${SOL2_INCLUDE_DIRS}
)

if (MSVC)
    add_compile_options(/bigobj)
else ()
    #add_compile_options(-Wa,-mbig-obj)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC ReactPhysics3D::ReactPhysics3D)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::enet::enet)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::lua::lua)
target_link_libraries(${PROJECT_NAME} PUBLIC Ore::library)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)

#target_link_libraries(${PROJECT_NAME} PUBLIC assimp)
#target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
#target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

