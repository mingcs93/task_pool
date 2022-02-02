# set minimum cmake version
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# project name and language
project(task_pool LANGUAGES CXX)
set(target task_pool)

# require C++17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


file(GLOB HEADER "*.h")
aux_source_directory(. SOURCES)

add_executable(${target} ${HEADER} ${SOURCES})