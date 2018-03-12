add_library (message SHARED
    messagemanager.cpp
    message.cpp
    )

target_link_libraries(message nanopb)
target_include_directories(message PRIVATE ${LUNA_INCLUDE_DIR})

set(libName libmessage.so)
add_deployment_file(${libName} lib/)
