project(Game)

set(MAIN "main" CACHE STRING "Select the program entry point")
set(SOURCEMAIN
    src/${MAIN}.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCEMAIN})

file(COPY ${PROJECT_SOURCE_DIR}/../res/ 
     DESTINATION ${PROJECT_BINARY_DIR}/../Game/res
)  

target_link_libraries(${PROJECT_NAME}  PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,Foundry::library>")
#target_link_libraries(${PROJECT_NAME} PRIVATE Foundry::library)
target_link_libraries(${PROJECT_NAME}  PRIVATE Editor::library)