FILE(GLOB_RECURSE LibFiles "*.h")

add_library (radio SHARED
    radio_manager.cpp
    radiorf24.cpp
    ${LibFiles}
    )

set(libName libradio.so)
add_deployment_file(${libName} lib/)
