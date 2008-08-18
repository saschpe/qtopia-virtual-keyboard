build_qtopia {
    qtopia_project(stub)
} else {
    message(Build keypad example for Qt or Qt/Embedded)
    TEMPLATE      = app
    TARGET        = keypad
    LIBS         += -lqtvirtualkeyboard

    FORMS        += keypad.ui
    RESOURCES    += keypad.qrc
    HEADERS      += keypad.h
    SOURCES      += main.cpp keypad.cpp
}
