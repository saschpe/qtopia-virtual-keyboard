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

#include "qvirtualkeyboardlayoutreader.h"
#include "qvirtualkeyboard.h"
#include "qvirtualkey.h"

#include <QIcon>

/*!
    \internal
    \class QVirtualKeyboardLayoutReader qvirtualkeyboardlayoutreader.h
    \brief
    \mainclass

    \sa QXmlStreamReader
*/

/*!
    \internal
    \brief Constructs
*/
QVirtualKeyboardLayoutReader::QVirtualKeyboardLayoutReader(QVirtualKeyboard *parent)
    : QXmlStreamReader()
    , parent(parent)
{
}

/*!
    \internal
    \brief Reads provided the virtual keyboard layout file.
*/
bool QVirtualKeyboardLayoutReader::read(QIODevice *device)
{
    setDevice(device);

    while (!atEnd()) {
        readNext();
        if (isStartElement()) {
            if (name() == "virtualkeyboardlayout") {
                parent->setLayoutVersion(attributes().value("version").toString().toInt());
                parent->setLayoutName(attributes().value("name").toString());

                while (!atEnd()) {
                    readNext();
                    if (isEndElement())
                        break;
                    if (isStartElement())
                        name() == "vkey" ? readVirtualKey() : readUnkownElement();
                }
            } else
                raiseError(QObject::tr("The file is not a virtual keyboard layout file."));
        }
    }
    return !error();
}

/*!
    \internal
    \brief Process a virtual key XML element and lookup/load the correponding virtual
           keyboard key.
*/
void QVirtualKeyboardLayoutReader::readVirtualKey()
{
    Q_ASSERT(isStartElement() && (name() == "vkey")); // More brackets for MSVC's strange operator precedence!

    QVirtualKey *vkey = parent->findVirtualKey(attributes().value("name").toString());

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement()) {
            if (vkey) {
                Qt::Key key = QVirtualKeyboard::stringToKey(attributes().value("key").toString());
                QString text = attributes().value("text").toString();

                if (name() == "default") {
                    vkey->setKey(key);
                    text.isEmpty() ? vkey->setText(QString(vkey->key())) : vkey->setText(text);
                    vkey->setIcon(QIcon(attributes().value("icon").toString()));
                } else if (name() == "shift") {
                    vkey->setShiftKey(key);
                    text.isEmpty() ? vkey->setShiftText(QString(vkey->shiftKey())) : vkey->setShiftText(text);
                    vkey->setShiftIcon(QIcon(attributes().value("icon").toString()));
                } else if (name() == "alt") {
                    vkey->setAltKey(key);
                    text.isEmpty() ? vkey->setAltText(QString(vkey->altKey())) : vkey->setAltText(text);
                    vkey->setAltIcon(QIcon(attributes().value("icon").toString()));
                } else if (name() == "altshift") {
                    vkey->setAltShiftKey(key);
                    text.isEmpty() ? vkey->setAltShiftText(QString(vkey->altShiftKey())) : vkey->setAltShiftText(text);
                    vkey->setAltShiftIcon(QIcon(attributes().value("icon").toString()));
                }
            }

            while (!atEnd()) {
                readNext();
                if (isEndElement())
                    break;
            }
        }
    }
}

/*!
    \internal
    \brief Read over an unkown XML element.
*/
void QVirtualKeyboardLayoutReader::readUnkownElement()
{
    Q_ASSERT(isStartElement());

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement())
            readUnkownElement();
    }
}
