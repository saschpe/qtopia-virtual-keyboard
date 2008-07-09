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

#ifndef QVIRTUALKEY_H
#define QVIRTUALKEY_H

#include "qvirtualkeyboardglobal.h"

#include <QAbstractButton>
#include <QStyle>

class QVirtualKeyPrivate;

class Q_QVK_EXPORT QVirtualKey : public QAbstractButton
{
    Q_OBJECT

    Q_ENUMS(LayoutHint)
    Q_PROPERTY(Qt::Key key READ key WRITE setKey)
    Q_PROPERTY(QString shiftText READ shiftText WRITE setShiftText)
    Q_PROPERTY(QIcon shiftIcon READ shiftIcon WRITE setShiftIcon)
    Q_PROPERTY(Qt::Key shiftKey READ shiftKey WRITE setShiftKey)
    Q_PROPERTY(QString altText READ altText WRITE setAltText)
    Q_PROPERTY(QIcon altIcon READ altIcon WRITE setAltIcon)
    Q_PROPERTY(Qt::Key altKey READ altKey WRITE setAltKey)
    Q_PROPERTY(QString altShiftText READ altShiftText WRITE setAltShiftText)
    Q_PROPERTY(QIcon altShiftIcon READ altShiftIcon WRITE setAltShiftIcon)
    Q_PROPERTY(Qt::Key altShiftKey READ altShiftKey WRITE setAltShiftKey)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
    Q_PROPERTY(LayoutHint layoutHint READ layoutHint WRITE setLayoutHint)
    Q_PROPERTY(Qt::Alignment alignmentHint READ alignmentHint WRITE setAlignmentHint)

public:
    enum LayoutHint { DefaultLayoutHint, EconomicLayoutHint };

    explicit QVirtualKey(QWidget *parent = 0, Qt::Key key = Qt::Key_unknown, const QString &text = "", const QIcon &icon = QIcon());
    virtual ~QVirtualKey();

    void setKey(Qt::Key key);
    Qt::Key key() const;

    void setShiftText(const QString &text);
    QString shiftText() const;
    void setShiftIcon(const QIcon &icon);
    QIcon shiftIcon() const;
    void setShiftKey(Qt::Key key);
    Qt::Key shiftKey() const;

    void setAltText(const QString &text);
    QString altText() const;
    void setAltIcon(const QIcon &icon);
    QIcon altIcon() const;
    void setAltKey(Qt::Key key);
    Qt::Key altKey() const;

    void setAltShiftText(const QString &text);
    QString altShiftText() const;
    void setAltShiftIcon(const QIcon &icon);
    QIcon altShiftIcon() const;
    void setAltShiftKey(Qt::Key key);
    Qt::Key altShiftKey() const;

    void setBackgroundBrush(const QBrush &brush);
    QBrush backgroundBrush() const;

    void setLayoutHint(LayoutHint layoutHint);
    LayoutHint layoutHint() const;

    void setAlignmentHint(Qt::Alignment alignmentHint);
    Qt::Alignment alignmentHint() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintSubElement(QPainter *painter, const QString &text, const QIcon &icon, const QRect &rect, uint tf, QStyle::State state);

    QVirtualKeyPrivate *d;
};

#endif
