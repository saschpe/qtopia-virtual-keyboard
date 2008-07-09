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

#include "keyboard.h"

#include <QCoreApplication>
#include <QDebug>

Keyboard::Keyboard(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    // Let the virtual keyboard watch the virtual keys in the layout and handle
    // incomming key events
    keyboard.addKeyContainer(keyboardFrame);
    connect(&keyboard, SIGNAL(keyEvent(QKeyEvent *)), this, SLOT(handleKeyEvent(QKeyEvent *)));

    // Add an event filter to see what key events arrive at the line edit
    textEdit->installEventFilter(this);
}

bool Keyboard::eventFilter(QObject *object, QEvent *event)
{
    // This eventFilter allows you to inspect the incomming key events.
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
        qDebug() << "Keyboard::eventFilter(" << object << "," << event << ")";
    return false;
}

void Keyboard::handleKeyEvent(QKeyEvent *event)
{
    // Note that you would send the event to QWS server under Qt/Embedded if
    // you want to use input methods together with the virtual keyboard.
    QCoreApplication::sendEvent(textEdit, event);
}
