
add_library(lib_qcmodel qcmodel.cpp qcmodel.h)

target_link_libraries(lib_qcmodel 
    ${Qt5Widgets_LIBRARIES}
    lib_src 
    lib_cjson
    )

