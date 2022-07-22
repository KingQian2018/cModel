
add_library(lib_qcmodel 
    qcmodel.cpp 
    qcmodel.h
    qcm_view.cpp 
    qcm_view.h
    qcm_scene.cpp 
    qcm_scene.h
    )

target_link_libraries(lib_qcmodel 
    ${Qt5Widgets_LIBRARIES}
    lib_src 
    lib_cjson
    )

