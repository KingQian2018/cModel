set(qcmodel
    qcm_pid.cpp
    qcm_pid.h
    )


add_library(lib_qcmodel 
    qcm.h
    qcm_window.cpp 
    qcm_window.h
    qcm_io.cpp 
    qcm_io.h
    qcmodel.cpp 
    qcmodel.h
    qcm_view.cpp 
    qcm_view.h
    qcm_scene.cpp 
    qcm_scene.h
    qcm_nodeline.cpp 
    qcm_nodeline.h
    qcm_node.cpp 
    qcm_node.h
    qcm_linknodes.cpp
    qcm_linknodes.h
    ${qcmodel}
    )

target_link_libraries(lib_qcmodel 
    ${Qt5Widgets_LIBRARIES}
    lib_src 
    lib_cjson
    )

