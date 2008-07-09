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

#include <QString>
#include <QIcon>
#include <QBrush>

#include "qvirtualkey.h"

class QVirtualKeyPrivate
{
public:
    QVirtualKeyPrivate()
        : key(Qt::Key_unknown)
        , shiftKey(Qt::Key_unknown)
        , altKey(Qt::Key_unknown)
        , altShiftKey(Qt::Key_unknown)
        , layoutHint(QVirtualKey::DefaultLayoutHint)
        , alignmentHint(Qt::AlignCenter | Qt::AlignAbsolute)
        , spacingHorizontal(2)
        , spacingVertical(2)
    {}

    Qt::Key key;

    QString shiftText;
    QIcon shiftIcon;
    Qt::Key shiftKey;

    QString altText;
    QIcon altIcon;
    Qt::Key altKey;

    QString altShiftText;
    QIcon altShiftIcon;
    Qt::Key altShiftKey;

    QBrush backgroundBrush;

    QVirtualKey::LayoutHint layoutHint;
    Qt::Alignment alignmentHint;

    const int spacingHorizontal;
    const int spacingVertical;
};
