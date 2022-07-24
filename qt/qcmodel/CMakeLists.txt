set(qcmodel
    qcm_pid.cpp
    qcm_pid.h
    )


add_library(lib_qcmodel 
    qcm.h
    qcm_window.cpp 
    qcm_window.h
    qcmodel.cpp 
    qcmodel.h
    qcm_view.cpp 
    qcm_view.h
    qcm_scene.cpp 
    qcm_scene.h
    qcm_nodeline.cpp 
    qcm_nodeline.h
    ${qcmodel}
    )

target_link_libraries(lib_qcmodel 
    ${Qt5Widgets_LIBRARIES}
    lib_src 
    lib_cjson
    )

