cmake_minimum_required(VERSION 3.10)

project(CPA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

include_directories(include ${JSONCPP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/InputReader.cpp
)

target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/students.json DESTINATION ${CMAKE_BINARY_DIR})
