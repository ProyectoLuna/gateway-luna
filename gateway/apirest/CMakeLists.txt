FILE(GLOB_RECURSE LibFiles "*.h")

add_library (apirest SHARED
    apirest.cpp
    ${LibFiles}
    )

target_link_libraries(apirest common)
target_include_directories(apirest PUBLIC ${CMAKE_SOURCE_DIR}/common)

set(libName libapirest.so)
add_deployment_file(${libName} lib/)
