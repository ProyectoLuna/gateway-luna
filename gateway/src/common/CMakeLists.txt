add_library (common SHARED
    servicebase.cpp
    )

target_include_directories(common PUBLIC ${LUNA_INCLUDE_DIR})

set(libName libcommon.so)
add_deployment_file(${libName} lib/)
