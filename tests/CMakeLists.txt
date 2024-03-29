set(unity_tests 
    io_test
    limit_test
    parse_test
)
add_library(unity STATIC unity/src/unity.c)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/outputs")
file(GLOB test_files "outputs/*")
file(COPY ${test_files} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/outputs/")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/parse_test")
file(GLOB test_parsefiles "parse_test/*")
file(REMOVE test_parsefiles "parse_test/*")
file(COPY ${test_parsefiles} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/parse_test/")

message(STATUS "tests: parse files ${test_parsefiles}")

foreach(unity_test ${unity_tests})
    add_executable("${unity_test}" "${unity_test}.c")
    target_link_libraries("${unity_test}" "${LIBS}" unity)
    add_test(NAME "${unity_test}"
        COMMAND "./${unity_test}")
endforeach(unity_test ${unity_tests})

add_dependencies(check ${unity_tests})
