FILE(GLOB_RECURSE LibFiles "*.h")

add_library (message SHARED
    messagemanager.cpp
    message.cpp
    ${LibFiles}
    )

target_link_libraries(message nanopb)
target_include_directories(message PRIVATE ${CMAKE_SOURCE_DIR}/protos/nanopb)

set(libName libmessage.so)
add_deployment_file(${libName} lib/)
