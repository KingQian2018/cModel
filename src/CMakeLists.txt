set(SRC_C 
    ./core/cm_common.c
    ./core/cm_parse.c
    ./core/io_port.c
    ./core/cm.c
    ./core/auto_generate_prase.c
    ./algo/pid.c
    ./algo/const.c
    ./algo/translate.c
    ./algo/limit.c
    ./algo/switch.c
    ./algo/gain.c
    ./hal/cm_hal_tick.c
)

add_library(lib_src ${SRC_C})
target_link_libraries(lib_src lib_easylogger)
