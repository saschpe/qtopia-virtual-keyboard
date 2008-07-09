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

#include "virtualkeyinterface.h"
#include "qvirtualkey.h"

#include <QtPlugin>

VirtualKeyInterface::VirtualKeyInterface(QObject *parent)
    : QObject(parent)
    , initialized(false)
{
}

void VirtualKeyInterface::initialize(QDesignerCustomWidgetInterface * /*core*/)
{
    if (initialized)
        return;
    initialized = true;
}

QWidget *VirtualKeyInterface::createWidget(QWidget *parent)
{
    return new QVirtualKey(parent);
}

QString VirtualKeyInterface::domXml() const
{
    return "<widget class=\"QVirtualKey\" name=\"Virtual Key\">\n"
           "  <property name=\"objectName\">\n"
           "    <string notr=\"true\">virtualKey</string>\n"
           "  </property>\n"
           "  <property name=\"geometry\">\n"
           "    <rect>\n"
           "      <x>0</x>\n"
           "      <y>0</y>\n"
           "      <width>36</width>\n"
           "      <height>36</height>\n"
           "    </rect>\n"
           "  </property>\n"
           "</widget>\n";
}
