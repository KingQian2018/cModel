
set(EASYLOGGER_C 
    ./port/elog_port.c
    ./src/elog_async.c
    ./src/elog_buf.c
    ./src/elog_utils.c
    ./src/elog.c
)

# include_directories(
#     ./
#     ./src/
# )

add_library(lib_easylogger ${EASYLOGGER_C})
