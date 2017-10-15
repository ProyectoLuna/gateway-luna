cmake_minimum_required(VERSION 2.8.12)

project(gateway)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)

include_directories("/opt/poky/2.3/metaqtsdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/")
include_directories("/opt/poky/2.3/metaqtsdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/")
include_directories("/opt/poky/2.3/metaqtsdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/QtCore")

find_package(Qt5Core)
add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5Core)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mfloat-abi=hard" )
