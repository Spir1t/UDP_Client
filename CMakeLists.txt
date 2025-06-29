cmake_minimum_required(VERSION 3.10) 
project(UDP_Client C)         

include_directories(${CMAKE_SOURCE_DIR}/h)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

message(STATUS "Cписок исходников: ${SOURCES}")

add_executable(UDP_Client ${SOURCES})