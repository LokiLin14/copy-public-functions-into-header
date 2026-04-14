cmake_minimum_required(VERSION 4.1)
project(copy-declarations C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/external/sds/sds.c
    ${PROJECT_SOURCE_DIR}/src/copy-declarations.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/src
)
