project(gateway)

set(
    SOURCES
    main.cpp
    daemon.cpp
    gateway.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${LUNA_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} Qt5Core)
target_link_libraries(${PROJECT_NAME} CuteLogger)
target_link_libraries(${PROJECT_NAME} radio)
target_link_libraries(${PROJECT_NAME} apirest)

add_deployment_file(${PROJECT_NAME} .)
