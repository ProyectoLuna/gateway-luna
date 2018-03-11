FILE(GLOB_RECURSE LibFiles "*.h")

add_library (common SHARED
    servicebase.cpp
    ${LibFiles}
    )

target_include_directories(radio PUBLIC ${CMAKE_SOURCE_DIR}/common)
set(libName libcommon.so)
add_deployment_file(${libName} lib/)
