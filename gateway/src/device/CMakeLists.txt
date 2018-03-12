add_library (device SHARED
    devicemanager.cpp
    device.cpp
    )

target_link_libraries(device nanopb)
target_include_directories(device PRIVATE ${LUNA_INCLUDE_DIR})

set(libName libdevice.so)
add_deployment_file(${libName} lib/)
