add_library (apirest SHARED
    apirest.cpp
    )

target_link_libraries(apirest common)
target_link_libraries(apirest Qt5Core)
target_link_libraries(apirest Qt5Network)
target_link_libraries(apirest qhttp)
target_include_directories(apirest PUBLIC ${LUNA_INCLUDE_DIR})

set(libName libapirest.so)
add_deployment_file(${libName} lib/)
