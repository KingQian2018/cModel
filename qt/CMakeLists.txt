

project(PlotData LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS PrintSupport Core Gui Widgets Sql Test Xml Network Concurrent)
find_package(Qt5LinguistTools)

set(TS_FILES study3_zh_CN.ts)
file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc)

set(QRC_SOURCE_FILES 
    qrc/main.qrc
    qrc/font.qrc)

qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})

if(ANDROID)
  add_library(PlotData SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
  )
else()
  add_executable(PlotData
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
    ${QRC_FILES}
  )
endif()

include_directories(frmcustom 3rd_qcustomplot 3rd_qcustomplot/v2_1)
add_subdirectory(frmcustom)
add_subdirectory(3rd_qcustomplot)
add_subdirectory(qcmodel)
target_link_libraries(PlotData
  lib_frmcustom
  lib_qcustomplot
  Qt5::Widgets
  lib_qcmodel)
message(STATUS "PlotData:${LIBS}")

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
