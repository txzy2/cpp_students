cmake_minimum_required(VERSION 3.10)

project(CPA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
        simdjson
        GIT_REPOSITORY https://github.com/simdjson/simdjson.git
        GIT_TAG v3.9.5
)
FetchContent_MakeAvailable(simdjson)

include_directories(include)

add_compile_options(-Wno-deprecated-literal-operator)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/InputReader.cpp
        include/Menus.h
)

target_link_libraries(${PROJECT_NAME} simdjson::simdjson)

file(COPY ${CMAKE_SOURCE_DIR}/students.json DESTINATION ${CMAKE_BINARY_DIR})
