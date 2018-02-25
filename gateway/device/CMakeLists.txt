FILE(GLOB_RECURSE LibFiles "*.h")

add_library (device SHARED
    devicemanager.cpp
    device.cpp
    ${LibFiles}
    )

target_link_libraries(device nanopb)
target_include_directories(device PRIVATE ${CMAKE_SOURCE_DIR}/protos/nanopb)

set(libName libdevice.so)
add_deployment_file(${libName} lib/)
