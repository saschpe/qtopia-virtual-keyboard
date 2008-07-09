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

#include "keypad.h"

#include <QCoreApplication>
#include <QMessageBox>

KeyPad::KeyPad(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    connect(enterButton, SIGNAL(clicked()), this, SLOT(accessCheck()));
    connect(layoutCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeKeyboardLayout()));

    // Let the virtual keyboard watch the virtual keys in the layout
    // and add a receiver to send key events to. The last thing we
    // do is to disable the default auto-shifting behavior for character
    // keys, we want to have them unchanged.
    keyboard.addKeyContainer(keyContainerBox);
    keyboard.setAutoShifting(false);
    connect(&keyboard, SIGNAL(keyEvent(QKeyEvent *)), this, SLOT(handleKeyEvent(QKeyEvent *)));

    // Set default keyboard layout map
    changeKeyboardLayout();
}

void KeyPad::accessCheck()
{
    // Highly sophisticated authentication scheme applied here ...
    if (lineEdit->text() == "42")
        QMessageBox::information(this, tr("Auth info"), tr("Valid key, access is granted!"));
    else
        QMessageBox::information(this, tr("Auth info"), tr("Invalid key, this incident was reported."));
}

void KeyPad::changeKeyboardLayout()
{
    bool success = false;

    // Change keyboard layout based on user selection
    if (layoutCombo->itemText(layoutCombo->currentIndex()) == tr("Numbers"))
        success = keyboard.setLayout(":numbers.qvkm");
    else if (layoutCombo->itemText(layoutCombo->currentIndex()) == tr("Characters"))
        success = keyboard.setLayout(":characters.qvkm");

    if (success)
        statusLabel->setText(tr("Keymap layout %1 loaded").arg(keyboard.layoutName()));
}

void KeyPad::handleKeyEvent(QKeyEvent *event)
{
    // Note that you would send the event to QWS server under Qt/Embedded if
    // you want to use input methods together with the virtual keyboard.
    QCoreApplication::sendEvent(lineEdit, event);
}
