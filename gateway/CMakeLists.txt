cmake_minimum_required(VERSION 2.8.12)

# Deployment macros
file(WRITE "${CMAKE_SOURCE_DIR}/QtCreatorDeployment.txt" "/home/root/\n")

macro(add_deployment_file SRC DEST)
    file(RELATIVE_PATH path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    file(APPEND "${CMAKE_SOURCE_DIR}/QtCreatorDeployment.txt" "${path}/${SRC}:${DEST}\n")
endmacro()

macro(add_deployment_directory SRC DEST)
    file(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC}/*")
    foreach(filename ${files})
        get_filename_component(path ${filename} PATH)
        add_deployment_file("${filename}" "${DEST}/${path}")
    endforeach(filename)
endmacro()

project(gateway)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/QtCore")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/QtNetwork")

add_executable(Qt5::moc IMPORTED)
set_target_properties(Qt5::moc PROPERTIES
    IMPORTED_LOCATION ${QTDIR}
)

find_package(Qt5Core)
find_package(Qt5Network)

set(
    SOURCES
    main.cpp
    daemon.cpp
    excpt.cpp
    gateway.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
SET(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")

add_subdirectory(radio)
INCLUDE_DIRECTORIES(radio)
add_subdirectory(protos)
INCLUDE_DIRECTORIES(protos/nanopb)
add_subdirectory(message)
INCLUDE_DIRECTORIES(message)
add_subdirectory(device)
INCLUDE_DIRECTORIES(device)
add_subdirectory(apirest)
INCLUDE_DIRECTORIES(apirest)
add_subdirectory(common)
INCLUDE_DIRECTORIES(common)

target_link_libraries(${PROJECT_NAME} Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5Network)
target_link_libraries(${PROJECT_NAME} rf24)
target_link_libraries(${PROJECT_NAME} rf24network)
target_link_libraries(${PROJECT_NAME} rf24mesh)
target_link_libraries(${PROJECT_NAME} CuteLogger)
target_link_libraries(${PROJECT_NAME} radio)
target_link_libraries(${PROJECT_NAME} nanopb)
target_link_libraries(${PROJECT_NAME} qhttp)
target_link_libraries(${PROJECT_NAME} apirest)
