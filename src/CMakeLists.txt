set(SRC_C 
    ./core/io_port.c
    ./core/cm.c
    ./algo/pid.c
    ./algo/const.c
    ./algo/translate.c
    ./hal/cm_hal_tick.c
)

include_directories(
    ../
    ./
    ./algo/
    ./core/
    ./hal/
    ./misc/
)

add_library(lib_src ${SRC_C})
