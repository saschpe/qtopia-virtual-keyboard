build_qtopia {
    message(Build sources for Qtopia)
    qtopia_project(subdirs)
} else {
    message(Build sources for Qt or Qt/Embedded)
    TEMPLATE = subdirs
}

# Mandatory: The Qt virtual keyboard library
SUBDIRS  = library

# Recommended: The Qt designer plugin
SUBDIRS += plugin
