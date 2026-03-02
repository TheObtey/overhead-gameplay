project(Game)

find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)

set(MAIN "main" CACHE STRING "Select the program entry point")

set(SOURCE
    src/${MAIN}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE})

file(COPY ${PROJECT_SOURCE_DIR}/../res/ 
     DESTINATION ${PROJECT_BINARY_DIR}/res  
)  

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(Game PRIVATE ${LUA_LIBRARIES})
target_link_libraries(Game PRIVATE sol2::sol2)

