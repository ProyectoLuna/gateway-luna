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

set(CMAKE_AUTOMOC ON)

add_executable(Qt5::moc IMPORTED)
set_target_properties(Qt5::moc PROPERTIES
    IMPORTED_LOCATION ${QTDIR}
)

find_package(Qt5Core)
find_package(Qt5Network)

include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/QtCore")
include_directories("/opt/poky/2.3/qt5sdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/qt5/QtNetwork")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")

set(LUNA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(src/gateway)
add_subdirectory(src/radio)
add_subdirectory(src/protos)
add_subdirectory(src/message)
add_subdirectory(src/device)
add_subdirectory(src/apirest)
add_subdirectory(src/common)
