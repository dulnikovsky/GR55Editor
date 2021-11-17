cmake_minimum_required(VERSION 3.5)

project(GR55Editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        gr55editorapplication.h
        gr55editorapplication.cpp
        midiportidalsa.h
        midiportidalsa.cpp
        alsafunc.h
        alsafunc.cpp
        midiinthread.h
        midiinthread.cpp
        midievent.h
        midievent.cpp
        patch.h
        patch.cpp
        datawrappers/patchnamewrapper.h
        datawrappers/patchnamewrapper.cpp
        datawrappers/patchdatawrapperbase.h
        datawrappers/patchdatawrapperbase.cpp
        datawrappers/patchpcmwrapper.h
        datawrappers/patchpcmwrapper.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GR55Editor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(GR55Editor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(GR55Editor
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(GR55Editor
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(GR55Editor
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick asound)

set_target_properties(GR55Editor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(QMLTest)
    qt_finalize_executable(QMLTest)
endif()
