cmake_minimum_required(VERSION 3.0...4.0)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ compiler" FORCE)
set(VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg-ports")
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")
set(CMAKE_CXX_STANDARD 20)
project(FoundryProject)

add_compile_definitions(_X86_)

file(COPY ${PROJECT_SOURCE_DIR}/res/ 
     DESTINATION ${PROJECT_BINARY_DIR}/res  
)  

# Indication to the code that this is a debug build
if (${IS_DEBUG_BUILD})
    add_compile_definitions(DEBUG_BUILD)
else()
    add_compile_definitions(RELEASE_BUILD)
endif()


add_subdirectory(Game)
add_subdirectory(Foundry)
add_subdirectory(Ore)
add_subdirectory(testLib)
add_subdirectory(RHI)

