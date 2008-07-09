build_qtopia {
    message(Build examples for Qtopia)
    qtopia_project(subdirs)
} else {
    message(Build examples for Qt or Qt/Embedded)
    TEMPLATE = subdirs
}

SUBDIRS  = keypad
SUBDIRS += keyboard
