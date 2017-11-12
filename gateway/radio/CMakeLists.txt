FILE(GLOB_RECURSE LibFiles "*.h")

add_library (Radio SHARED
    radio_manager.cpp
    radiorf24.cpp
    ${LibFiles}
    )
