set(frmcustoms 
    ./iconhelper.cpp
    ./frmrealtimedata.cpp
    ./frmrealtimedata.h
    ./frmrealtimedata.ui
    )

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)


add_library(lib_frmcustom ${frmcustoms})
target_link_libraries(lib_frmcustom ${Qt5Widgets_LIBRARIES} lib_qcustomplot)

