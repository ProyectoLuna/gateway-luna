include(ExternalProject)

ExternalProject_Add(nanopd-generator-bin
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/protos/nanopb
        URL http://localhost:8081/artifactory/generic-local/generator-bin.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )

set(protosFile ${CMAKE_CURRENT_SOURCE_DIR}/../remote_devs.proto)
set(genFile1 ${CMAKE_CURRENT_SOURCE_DIR}/remote_devs.pb.c)

# Autogenerate nanopb files
if(${protosFile} IS_NEWER_THAN ${genFile1})
    message(STATUS "execute protoc cmd")
    set (protoc_cmd "${CMAKE_CURRENT_SOURCE_DIR}/src/nanopd-generator-bin/protoc")
    set (protoc_arg1 "--nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}")
    set (protoc_arg2 "--proto_path=${CMAKE_CURRENT_SOURCE_DIR}/..")
    set (protoc_arg3 "${CMAKE_CURRENT_SOURCE_DIR}/../remote_devs.proto")
    message (STATUS "${protoc_cmd} ${protoc_arg1} ${protoc_arg2} ${protoc_arg3}")
    execute_process(COMMAND ${protoc_cmd} ${protoc_arg1} ${protoc_arg2} ${protoc_arg3}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protos/nanopb
                    RESULT_VARIABLE protoc_result)
    message(STATUS "protoc [${protoc_result}]")

    if(${protoc_result} GREATER "0")
        message(STATUS "ERROR GENERATING protos files")
    endif(${protoc_result} GREATER "0")
endif()

add_library (nanopb SHARED
    pb_common.c
    pb_decode.c
    pb_encode.c
    remote_devs.pb.c
    )

target_include_directories(nanopb PUBLIC ${LUNA_INCLUDE_DIR})
target_include_directories(nanopb PUBLIC ${LUNA_INCLUDE_DIR}/protos/nanopb)

set(libName libnanopb.so)
add_deployment_file(${libName} lib/)
