FILE(GLOB_RECURSE LibFiles "*.h")

add_library (radio SHARED
    radio_manager.cpp
    radiorf24.cpp
    ${LibFiles}
    )

target_link_libraries(radio nanopb)
target_include_directories(radio PRIVATE ${CMAKE_SOURCE_DIR}/protos/nanopb)
target_link_libraries(radio common)
target_include_directories(radio PRIVATE ${CMAKE_SOURCE_DIR}/common)

set(libName libradio.so)
add_deployment_file(${libName} lib/)
