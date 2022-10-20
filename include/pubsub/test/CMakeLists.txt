project(pubsub_test)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/test/pubsub)

list(APPEND
    cpp_file_names
    test_pub
    test_sub
)

set(dep_libs
    spdlog
    hiredis
    redis++
)

foreach(cpp_file_name ${cpp_file_names})
    add_executable(${cpp_file_name} ${cpp_file_name}.cpp)
    target_link_libraries(${cpp_file_name} PRIVATE ${dep_libs})
endforeach()