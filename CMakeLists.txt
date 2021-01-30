cmake_minimum_required(VERSION 3.0.0)
project(sample-agent VERSION 0.0.1)

add_subdirectory(modules/alica-message-protos)
add_subdirectory(modules/capnzero/capnzero)

file(GLOB_RECURSE SOURCES "src/*")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        modules/capnzero/capnzero/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        capnzero
        alica-message-protos
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        std_cxx_17
)

install(TARGETS ${PROJECT_NAME} capnzero
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
