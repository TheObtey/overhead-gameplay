project(Game)

set(MAIN "main" CACHE STRING "Select the program entry point")

set(SOURCE
    src/${MAIN}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

file(COPY ${PROJECT_SOURCE_DIR}/../res/ 
     DESTINATION ${PROJECT_BINARY_DIR}/res  
)  

target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)

