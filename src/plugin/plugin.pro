build_qtopia {
    qtopia_project(stub)
} else {
    message(Build designer plugin for Qt or Qt/Embedded)
    TEMPLATE     = lib
    TARGET       = qtvirtualkeyboardplugin
    CONFIG      += designer plugin release
    DEFINES     += QT_NO_DEBUG_OUTPUT

    INCLUDEPATH += ../library
    LIBS        += -L../library -lqtvirtualkeyboard

    RESOURCES   += virtualkeyplugin.qrc
    HEADERS     += virtualkeyplugin.h \
                   virtualkeyinterface.h
    SOURCES     += virtualkeyplugin.cpp \
                   virtualkeyinterface.cpp

    target.path  = $$[QT_INSTALL_PLUGINS]/designer
    INSTALLS    += target
}
