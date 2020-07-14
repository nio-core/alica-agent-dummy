cmake_minimum_required(VERSION 3.0.0)
project(sample-agent VERSION 0.0.1)

add_subdirectory(modules/capnzero/capnzero)
file(GLOB PROTOS
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/alica-message-protos/proto/*.capnp
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/capnzero/id_capnzero/include/capnzero/*.capnp
        )
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/proto)
foreach(PROTO ${PROTOS})
        execute_process(
                COMMAND bash -c "capnp compile \
                         --import-path=${CMAKE_CURRENT_SOURCE_DIR}/modules/capnzero/id_capnzero/include/ \
                         --src-prefix=${CMAKE_CURRENT_SOURCE_DIR}/modules/alica-message-protos/proto \
                         --src-prefix=${CMAKE_CURRENT_SOURCE_DIR}/modules/capnzero/id_capnzero/include/ \
                         --output=c++:${CMAKE_CURRENT_SOURCE_DIR}/proto \
                         '${PROTO}'"
        )
endforeach()

file(GLOB_RECURSE SOURCES "src/*" "include/*.c++")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
        proto
    PRIVATE
        modules/capnzero/capnzero/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        capnzero
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        std_cxx_17
)

install(TARGETS ${PROJECT_NAME} capnzero
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
