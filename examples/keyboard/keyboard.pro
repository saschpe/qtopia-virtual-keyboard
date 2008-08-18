build_qtopia {
    qtopia_project(stub)
} else {
    message(Build keyboard example for Qt or Qt/Embedded)
    TEMPLATE     = app
    TARGET       = keyboard
    LIBS        += -lqtvirtualkeyboard

    FORMS       += keyboard.ui
    HEADERS     += keyboard.h
    SOURCES     += main.cpp keyboard.cpp
}
