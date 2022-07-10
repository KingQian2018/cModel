set(unity_tests 
    io_test
    limit_test
    parse_test
)
add_library(unity STATIC unity/src/unity.c)
add_library(glad STATIC glad/src/glad.c)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/outputs")
file(GLOB test_files "outputs/*")
file(COPY ${test_files} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/outputs/")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/parse_test")
file(GLOB test_parsefiles "parse_test/*")
file(REMOVE test_parsefiles "parse_test/*")
file(COPY ${test_parsefiles} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/parse_test/")

file(GLOB test_glfw "glfw-3.3.7.bin.WIN64/lib-mingw-w64/*")
file(COPY ${test_glfw} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

link_directories("${PROJECT_SOURCE_DIR}/tests/glfw-3.3.7.bin.WIN64/lib-mingw-w64" )

message(STATUS "KW: ${PROJECT_SOURCE_DIR}/tests/glfw-3.3.7.bin.WIN64/lib-mingw-w64")
foreach(unity_test ${unity_tests})
    add_executable("${unity_test}" "${unity_test}.c")
    target_link_libraries("${unity_test}" "${LIBS}" unity)
    add_test(NAME "${unity_test}"
        COMMAND "./${unity_test}")
endforeach(unity_test ${unity_tests})

target_link_libraries(parse_test glad glfw3)

add_dependencies(check ${unity_tests})
