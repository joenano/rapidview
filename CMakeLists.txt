cmake_minimum_required(VERSION 3.5)

project(dayjur VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(UI
    src/ui/mainwindow.ui
    src/ui/jsonview.ui
)

set(HEADER
    src/jsonview.h
    src/mainwindow.h
    src/tabs.h
)

set(SRC
    src/jsonview.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/tabs.cpp
)

set(PROJECT_SOURCES
    ${HEADER}
    ${SRC}
    ${UI}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dayjur
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(dayjur
        ${PROJECT_SOURCES}
        resource.qrc
    )
endif()

target_link_libraries(dayjur PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS dayjur
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dayjur)
endif()
