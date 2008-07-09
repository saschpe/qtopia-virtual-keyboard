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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "ui_keyboard.h"
#include <qtvirtualkeyboard/qvirtualkeyboard.h>
#include <QMainWindow>

class Keyboard : public QMainWindow, private Ui::Keyboard
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void handleKeyEvent(QKeyEvent *);

private:
    QVirtualKeyboard keyboard;
};

#endif
