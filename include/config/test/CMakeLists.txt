project(yaml_test)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/test/yaml)

list(APPEND
    cpp_file_names
    test_yaml
)

set(dep_libs
    spdlog
    yaml-cpp
)

foreach(cpp_file_name ${cpp_file_names})
    add_executable(${cpp_file_name} ${cpp_file_name}.cpp)
    target_link_libraries(${cpp_file_name} PRIVATE ${dep_libs})
endforeach()