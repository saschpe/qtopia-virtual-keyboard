README
======

This document gives general information about the partner solution.


Provider
========

Trolltech


Functionality overview
======================

Small library which provides virtual keyboard functionality to Qt.


Benefits
========

The solution is small, simple and can be visually designed and translated. It is
based only on QObject event handling and thus can be used for Qt, Qt/Embedded or Qtopia.
To get a feeling of it's benefits, have a look at the examples.


Content of delivery
===================

The library is delived as source together with a designer plugin and examples.

src/library   - Complete library source code (ready to be deployed)
src/plugin    - Complete designer plugin source code (optional)
src/examples  - Examples (optional)
docs/         - API documentation (see GS_INSTALL.txt for details)


Other available documents
=========================

GS_INSTALL.txt       - Installation guide
GS_RELEASE_NOTES.txt - Release Notes
docs/                - API documentation (see GS_INSTALL.txt for details)


User guide
==========

The virtual keyboard library has to be built from source and installed into the systems 
library path to be useable by the examples and your own code. A virtual keyboad layout
should be developed by the help of designer (see examples) not via source code.

Have a look also at the API documentation to get further information on how to use the library.


Required environment
====================

Qt-4.x + SDK or
Qt/Embedded-4.x + SDK or
QPE-4.3.1 + SDK
