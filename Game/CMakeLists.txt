project(Game)

set(MAIN "main" CACHE STRING "Select the program entry point")

set(SOURCE
    src/${MAIN}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)

