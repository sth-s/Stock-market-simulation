cmake_minimum_required(VERSION 3.16)
project(advprog2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

find_package(nlohmann_json 3.2.0 REQUIRED)


add_executable(simulation simulation.cpp)

set(FUNC_LIB helper)
target_link_libraries(simulation PRIVATE
        ${FUNC_LIB}
        nlohmann_json::nlohmann_json
)
add_subdirectory(${FUNC_LIB})