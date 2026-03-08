cmake_minimum_required(VERSION 4.1)
project(copy-declarations C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME}
    src/main.c
    external/sds/sds.c
    src/copy-declarations.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ./external
    ./src
)
