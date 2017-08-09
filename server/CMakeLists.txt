cmake_minimum_required(VERSION 3.3)
project(internalServiceProvider)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -pthread -Wall -Werror")
set(ELEGANCE_SRC_PATH "../../Elegance/")
set(ELEGANCE_LIB_PATH "../../Elegance/lib/linux")

file(GLOB_RECURSE SERVER_CENTER_FILES ../common/*.cpp src/center/*.cpp server_center.cpp)
file(GLOB_RECURSE SERVER_GAME_OPERATOR_FILES ../common/*.cpp src/game_operator/*.cpp server_game_operator.cpp)

LINK_DIRECTORIES(${ELEGANCE_LIB_PATH})

add_executable(server_center ${SERVER_CENTER_FILES})
add_executable(server_game_operator ${SERVER_GAME_OPERATOR_FILES})

target_include_directories(server_center PUBLIC ${ELEGANCE_SRC_PATH} ../common/ src/center )
target_include_directories(server_game_operator PUBLIC ${ELEGANCE_SRC_PATH} ../common/ src/game_operator)

target_link_libraries(server_center libElegance.so)
target_link_libraries(server_game_operator libElegance.so)


