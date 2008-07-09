build_qtopia {
    message(Build for Qtopia)
    qtopia_project(subdirs)
} else {
    message(Build for Qt or Qt/Embedded)
    TEMPLATE = subdirs
}

SUBDIRS = src
