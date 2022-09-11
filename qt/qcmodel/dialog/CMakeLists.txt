find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)


add_library(lib_qcm_dialog
    perferencesDialog.cpp
    perferencesDialog.h
    qcm_dialog.h
    )


target_link_libraries(lib_qcm_dialog
    ${Qt5Widgets_LIBRARIES}
    )

