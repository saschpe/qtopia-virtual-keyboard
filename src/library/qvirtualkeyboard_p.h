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
#include <QHash>
#include <QList>

class QVirtualKeyboardPrivate
{
public:
    QVirtualKeyboardPrivate()
        : shiftModifier(Qt::Key_Shift)
        , altModifier(Qt::Key_AltGr)
        , rememberedStandardModifiers(Qt::NoModifier)
        , autoShifting(true)
        , autoShiftingMark(false)
        , deadKeys(true)
        , capsLock(true)
        , lastDeadKey(Qt::Key_unknown)
        , keyboardLayoutVersion(1)
        , keyboardLayoutName("Custom")
    {}

    QHash<QObject *, QList<QVirtualKey *> > virtualKeyHash;
    QHash<Qt::Key, int> currentModifierHash;

    Qt::Key shiftModifier; ///< Stores the current 'shift' modifier
    Qt::Key altModifier; ///< Stores the current 'alt' modifier
    Qt::KeyboardModifiers rememberedStandardModifiers;

    uint autoShifting : 1; ///< Determines if auto-shifting is enabled
    uint autoShiftingMark : 1; ///< Temporary marker for internal use
    uint deadKeys : 1; ///< Determines if dead keys are enabled
    uint capsLock : 1; ///< Determines if caps-lock behavior is enabled

    Qt::Key lastDeadKey; ///< Stores the last dead key for internal use
    int keyboardLayoutVersion; ///< Information about the current layout
    QString keyboardLayoutName; ///< Information about the current layout
};
