cmake_minimum_required(VERSION 3.0.0)
project(sample-agent VERSION 0.0.1)

add_subdirectory(dependency/alica-messages)
add_subdirectory(dependency/capnzero-monitoring)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        dependency/alica-messages/include
        dependency/capnzero-monitoring/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        capnzero-monitoring
        alica-messages
        capnzero
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        std_cxx_17
)
