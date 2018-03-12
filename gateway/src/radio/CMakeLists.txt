add_library (radio SHARED
    radio_manager.cpp
    radiorf24.cpp
    iradio.h
    )

target_include_directories(radio PUBLIC ${LUNA_INCLUDE_DIR})

target_link_libraries(radio Qt5Core)
target_link_libraries(radio nanopb)
target_link_libraries(radio common)
target_link_libraries(radio message)
target_link_libraries(radio rf24)
target_link_libraries(radio rf24network)
target_link_libraries(radio rf24mesh)

set(libName libradio.so)
add_deployment_file(${libName} lib/)
