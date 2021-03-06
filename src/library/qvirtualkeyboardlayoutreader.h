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

#ifndef QVIRTUALKEYBOARDLAYOUTREADER_H
#define QVIRTUALKEYBOARDLAYOUTREADER_H

#include <QXmlStreamReader>

class QIODevice;
class QVirtualKeyboard;

class QVirtualKeyboardLayoutReader : public QXmlStreamReader
{
public:
    explicit QVirtualKeyboardLayoutReader(QVirtualKeyboard *parent);

    bool read(QIODevice *device);

private:
    void readVirtualKey();
    void readUnkownElement();

    QVirtualKeyboard *parent;
};

#endif
