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

#ifndef VIRTUALKEYINTERFACE_H
#define VIRTUALKEYINTERFACE_H

#include <QDesignerCustomWidgetInterface>

class VirtualKeyInterface : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    VirtualKeyInterface(QObject *parent = 0);

    void initialize(QDesignerCustomWidgetInterface *core);
    bool isInitialized() const { return initialized; }

    QWidget *createWidget(QWidget *parent);

    bool isContainer() const { return false; }
    QString domXml() const;
    QString group() const { return "Buttons"; }
    QString includeFile() const { return "qtvirtualkeyboard/qvirtualkey.h"; }
    QIcon icon() const { return QIcon(":virtualkey.png"); }
    QString name() const { return "QVirtualKey"; }
    QString toolTip() const { return ""; }
    QString whatsThis() const { return ""; }

private:
    bool initialized;
};

#endif
