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

#ifndef VIRTUALKEYPLUGIN_H
#define VIRTUALKEYPLUGIN_H

#include <QtDesigner/QtDesigner>
#include <QtPlugin>

class VirtualKeyPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    VirtualKeyPlugin(QObject *parent = 0);

    virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface *> widgets;
};

#endif
