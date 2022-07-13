set(qcustomplot
    v2_1/qcustomplot.cpp
    v2_1/qcustomplot.h
    smoothcurve.cpp)

if( WIN32 )
    add_definitions( -DUNICODE )
endif()

add_library(lib_qcustomplot STATIC ${qcustomplot})
set_target_properties( lib_qcustomplot PROPERTIES AUTOMOC TRUE )
target_link_libraries(lib_qcustomplot Qt5::Widgets Qt5::Network Qt5::PrintSupport)
