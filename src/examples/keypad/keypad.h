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

#ifndef KEYPAD_H
#define KEYPAD_H

#include "ui_keypad.h"
#include <qtvirtualkeyboard/qvirtualkeyboard.h>
#include <QWidget>

class KeyPad : public QWidget, private Ui::KeyPad
{
    Q_OBJECT

public:
    explicit KeyPad(QWidget *parent = 0);

private slots:
    void accessCheck();
    void changeKeyboardLayout();
    void handleKeyEvent(QKeyEvent *);

private:
    QVirtualKeyboard keyboard;
};

#endif
