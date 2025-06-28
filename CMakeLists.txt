cmake_minimum_required(VERSION 3.10) 
project(CPU_Scanner C)         

# Указываем компилятору искать заголовочные файлы в папке h
include_directories(${CMAKE_SOURCE_DIR}/h)

# Собираем все исходники из папки src
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

message(STATUS "Cписок исходников: ${SOURCES}")

# Создаем исполняемый файл из исходников
add_executable(CPU_Scanner ${SOURCES})