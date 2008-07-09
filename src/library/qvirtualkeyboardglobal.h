/****************************************************************************
  **
  ** Copyright (C) 1992-$THISYEAR$ $TROLLTECH$. All rights reserved.
  **
  ** This file is part of the $MODULE$ of the Qt Toolkit.
  **
  ** $TROLLTECH_DUAL_LICENSE$
  **
  ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
  ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
  **
  ****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef BUILD_QVK
#  define Q_QVK_EXPORT Q_DECL_EXPORT
#else
#  define Q_QVK_EXPORT Q_DECL_IMPORT
#endif

#endif
