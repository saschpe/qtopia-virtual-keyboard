build_qtopia {
    message(Build library for Qtopia)
    qtopia_project(lib)
} else {
    message(Build library for Qt or Qt/Embedded)
    TEMPLATE  = lib
}

TARGET        = qtvirtualkeyboard
CONFIG       += shared release
DEFINES      += BUILD_QVK QT_NO_DEBUG_OUTPUT

HEADERS       = qvirtualkeyboardglobal.h \
                qvirtualkeyboard.h \
                qvirtualkey.h
SOURCES       = qvirtualkeyboard.cpp \
                qvirtualkey.cpp \
                qvirtualkeyboardlayoutreader.cpp

build_qtopia {
    resolve_include()

    # NOTE: Install headers into $$QPEDIR/include
    headers.files = $$HEADERS
    headers.path  = /../include/$$TARGET
    INSTALLS     += headers

    idep(LIBS+=-l$$TARGET)
    qt_inc()
} else {
    # NOTE: Changing include paths means changing it in 
    #       the designer plugin too!
    headers.files = $$HEADERS
    headers.path  = $$[QT_INSTALL_HEADERS]/$$TARGET
    INSTALLS     += headers

    target.path   = $$[QT_INSTALL_LIBS]
    INSTALLS     += target
}
