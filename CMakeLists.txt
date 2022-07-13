cmake_minimum_required(VERSION 3.5)

project(cModel VERSION 0.1.0)

include(CTest)
enable_testing()

set(EASYLOGGER_INC 
    ./component/easylogger/
    ./component/easylogger/src/
)

set(SEC_INC
    ./
    ./src/
    ./src/algo/
    ./src/core/
    ./src/hal/
    ./src/misc/
)

set(TESTS_INC
    ./tests/glad/include/
    ./tests/glfw-3.3.7.bin.WIN64/include/
)

include_directories(${EASYLOGGER_INC} ${SEC_INC} ${TESTS_INC})

add_subdirectory(component/easylogger/)
add_subdirectory(component/cJson/)
add_subdirectory(src/)
add_subdirectory(qt)

set(LIBS lib_src lib_cjson)
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

set(CMAKE_C_STANDARD, 99)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(tests/)
