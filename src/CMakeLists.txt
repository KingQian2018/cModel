set(SRC_C 
    ./core/io_port.c
    ./core/cm.c
    ./core/pid.c
    ./hal/cm_hal_tick.c
)

include_directories(
    ./
    ../
    ./core/
    ./hal/
    ./misc/
)

add_library(lib_src ${SRC_C})
