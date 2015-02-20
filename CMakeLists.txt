cmake_minimum_required(VERSION 2.8.4)
project(lightweight_logger)

set(EXEC_NAME lightweight_logger)

find_package(Threads) #pthread

set(INCLUDE_DIR ./include)

include_directories(${INCLUDE_DIR})

set(SOURCE_FILES
    main.cpp
    ./src/lightweight_logger.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${EXEC_NAME} ${SOURCE_FILES})

target_link_libraries(${EXEC_NAME} ${CMAKE_THREAD_LIBS_INIT})