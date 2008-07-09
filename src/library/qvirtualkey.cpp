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

#include "qvirtualkey.h"

#include <QPainter>
#include <QStyleOptionButton>
#include <QFontMetrics>

#include "qvirtualkey_p.h"

/*!
    \class QVirtualKey qvirtualkey.h
    \brief A widget resembling a standard keyboard button to be used with a virtual keyboard.
    \mainclass

    The virtual key has 3 key values available, which can be triggered (send as key
    event) by virtual modifier keys. Example:

    Let's assume you have configured a virtual key to look like this:

    \code
        ______
       | =    |
       | 0  } |
        ------
    \endcode

    By default the "=" will be send if this key and a modifier key which generates
    Qt::ShiftModifier are pressed together. This behavior can be totally customized,
    it is possible to have "=" only send, if several modifiers keys are pressed at
    the same time.

    \sa QVirtualKeyboard
*/

/*!
    \property QVirtualKey::key
    \brief The default key code associated with this virtual key.

    This property's default is Qt::Key_unknown

    \sa Qt::Key
*/

/*!
    \property QVirtualKey::shiftKey
    \brief The shift key code associated with this virtual key.

    This key code is generated when pressed together with a 'shift' modifier.

    This property's default is Qt::Key_unknown

    \sa Qt::Key, QVirtualKeyboard
*/

/*!
    \property QVirtualKey::shiftText
    \brief Used to display the visual meaning of the key.

    This is only the displayed text, not a keycode and can be superseeded if the
    corresponding icon is set.
*/

/*!
    \property QVirtualKey::shiftIcon
    \brief Used to display the visual meaning of the key.

    The icon's default size is defined by the GUI style, but can be
    adjusted by setting the \l iconSize property.
*/

/*!
    \property QVirtualKey::altKey
    \brief The alt key code associated with this virtual key.

    This key code is generated when pressed together with a 'alt' modifier.

    This property's default is Qt::Key_unknown

    \sa Qt::Key, QVirtualKeyboard
*/

/*!
    \property QVirtualKey::altText
    \brief Used to display the visual meaning of the key.

    This is only the displayed text, not a keycode and can be superseeded if the
    corresponding icon is set.
*/

/*!
    \property QVirtualKey::altIcon
    \brief

    The icon's default size is defined by the GUI style, but can be
    adjusted by setting the \l iconSize property.
*/

/*!
    \property QVirtualKey::altShiftKey
    \brief The alt-shift key code associated with this virtual key.

    This key code is generated when pressed together with a 'alt' and a
    'shift' modifier.

    This property's default is Qt::Key_unknown

    \sa Qt::Key, QVirtualKeyboard
*/

/*!
    \property QVirtualKey::altShiftText
    \brief Used to display the visual meaning of the key.

    This is only the displayed text, not a keycode and can be superseeded if the
    corresponding icon is set.
*/

/*!
    \property QVirtualKey::altShiftIcon
    \brief

    The icon's default size is defined by the GUI style, but can be
    adjusted by setting the \l iconSize property.
*/

/*!
    \property QVirtualKey::backgroundBrush
    \brief The background brush determines how the virtual key's background is
           painted.

    This property can be used together with font properties to customize the
    visual appearance of the widget. You can further fine-tune the appearance
    by using custom icons for the virtual key.

    \sa QBrush
*/

/*!
    \property QVirtualKey::layoutHint
    \brief

*/

/*!
    \brief Constructs a virtual key with \a text, \a icon and \a key and a \a parent.
*/
QVirtualKey::QVirtualKey(QWidget *parent, Qt::Key key, const QString &text, const QIcon &icon)
    : QAbstractButton(parent)
    , d(new QVirtualKeyPrivate)
{
    d->key = key;
    setText(text);
    setIcon(icon);
    setAutoRepeat(true);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    QFont f = font();
    f.setBold(true);
    setFont(f);
    d->backgroundBrush = palette().midlight();
}

/*!
    \brief Destroys the virtual key.
*/
QVirtualKey::~QVirtualKey()
{
    delete d;
}

/*!
    \brief Change the default key mapping.
*/
void QVirtualKey::setKey(Qt::Key key)
{
    d->key = key;
}

/*!
    \brief Retrieve the currently set default key mapping.

    \sa Qt::Key
*/
Qt::Key QVirtualKey::key() const
{
    return d->key;
}

/*!
    \brief Sets the text that is displayed up left inside the widget.

    This is mostly used    (but not mandatory) together with the shift key.
*/
void QVirtualKey::setShiftText(const QString &text)
{
    d->shiftText = text;
    update();
    updateGeometry();
}

/*
    \brief Retrieves the text that is displayed up left inside the widget.
*/
QString QVirtualKey::shiftText() const
{
    return d->shiftText;
}

/*!
    \brief Sets the icon that is displayed up left inside the widget

    This is mostly used (but not mandatory) together with the shift key.
*/
void QVirtualKey::setShiftIcon(const QIcon &icon)
{
    d->shiftIcon = icon;
    update();
    updateGeometry();
}

/*!
    \brief
*/
QIcon QVirtualKey::shiftIcon() const
{
    return d->shiftIcon;
}

/*!
    \brief
*/
void QVirtualKey::setShiftKey(Qt::Key key)
{
    d->shiftKey = key;
}

/*!
    \brief
*/
Qt::Key QVirtualKey::shiftKey() const
{
    return d->shiftKey;
}

/*!
    \brief Sets the text that is displayed down right inside the widget

    This is mostly used (but not mandatory) together with the alt key.
*/
void QVirtualKey::setAltText(const QString &text)
{
    d->altText = text;
    update();
    updateGeometry();
}

/*!
    \brief
*/
QString QVirtualKey::altText() const
{
    return d->altText;
}

/*!
    \brief Sets the icon that is displayed down right inside the widget

    This is mostly used (but not mandatory) together with the alt key.
*/
void QVirtualKey::setAltIcon(const QIcon &icon)
{
    d->altIcon = icon;
    update();
    updateGeometry();
}

/*!
    \brief
*/
QIcon QVirtualKey::altIcon() const
{
    return d->altIcon;
}

/*!
    \brief
*/
void QVirtualKey::setAltKey(Qt::Key key)
{
    d->altKey = key;
}

/*!
    \brief
*/
Qt::Key QVirtualKey::altKey() const
{
    return d->altKey;
}

/*!
    \brief Sets the text that is displayed up right inside the widget

    This is mostly used (but not mandatory) together with the alt-shift key.
*/
void QVirtualKey::setAltShiftText(const QString &text)
{
    d->altShiftText = text;
    update();
    updateGeometry();
}

/*!
    \brief
*/
QString QVirtualKey::altShiftText() const
{
    return d->altShiftText;
}

/*!
    \brief Sets the icon that is displayed up right inside the widget

    This is mostly used (but not mandatory) together with the alt-shift key.
*/
void QVirtualKey::setAltShiftIcon(const QIcon &icon)
{
    d->altShiftIcon = icon;
    update();
    updateGeometry();
}

/*!
    \brief
*/
QIcon QVirtualKey::altShiftIcon() const
{
    return d->altShiftIcon;
}

/*!
    \brief
*/
void QVirtualKey::setAltShiftKey(Qt::Key key)
{
    d->altShiftKey = key;
}

/*!
    \brief
*/
Qt::Key QVirtualKey::altShiftKey() const
{
    return d->altShiftKey;
}

/*!
    \brief Sets a custom brush for background rendering.
*/
void QVirtualKey::setBackgroundBrush(const QBrush &brush)
{
    d->backgroundBrush = brush;
    update();
}

/*!
    \brief Retrieves the current background brush.
*/
QBrush QVirtualKey::backgroundBrush() const
{
    return d->backgroundBrush;
}

/*!
    \brief
*/
void QVirtualKey::setLayoutHint(LayoutHint layoutHint)
{
    d->layoutHint = layoutHint;
    update();
}

/*!
    \brief
*/
QVirtualKey::LayoutHint QVirtualKey::layoutHint() const
{
    return d->layoutHint;
}

/*!
    \brief
*/
void QVirtualKey::setAlignmentHint(Qt::Alignment alignmentHint)
{
    d->alignmentHint = alignmentHint;
    update();
}

/*!
    \brief
*/
Qt::Alignment QVirtualKey::alignmentHint() const
{
    return d->alignmentHint;
}

/*! \reimp */
QSize QVirtualKey::sizeHint() const
{
    ensurePolished();
    QFontMetrics fm = fontMetrics();
    QSize size(d->spacingHorizontal, d->spacingVertical);
    QSize defaultSize(0, 0), shiftSize(0, 0), altSize(0, 0), altShiftSize(0, 0);

    QStyleOptionButton button;
    button.initFrom(this);
    int tf =  Qt::TextShowMnemonic;
    if (!style()->styleHint(QStyle::SH_UnderlineShortcut, &button, this))
        tf |= Qt::TextHideMnemonic;

    if (!icon().isNull())
        defaultSize = QSize(iconSize().width(), iconSize().height() + 2);
    else if (!text().isEmpty())
        defaultSize = fm.size(tf, text() + " ");

    if (!d->shiftIcon.isNull())
        shiftSize = QSize(iconSize().width(), iconSize().height() + 2);
    else if (!d->shiftText.isEmpty())
        shiftSize = fm.size(tf, d->shiftText + " ");

    if (!d->altIcon.isNull())
        shiftSize = QSize(iconSize().width(), iconSize().height() + 2);
    else if (!d->altText.isEmpty())
        altSize = fm.size(tf, d->altText + " ");

    if (!d->altShiftIcon.isNull())
        altShiftSize = QSize(iconSize().width(), iconSize().height() + 2);
    else if (!d->altShiftText.isEmpty())
        altShiftSize = fm.size(tf, d->altShiftText + " ");

    if (d->layoutHint == DefaultLayoutHint) {
        // To have the correct widget size, we have to set some sizes
        // even if the corresponding parts display nothing

        if (defaultSize.isEmpty() && shiftSize.isEmpty()) {
            if (!altSize.isEmpty())
                defaultSize = altSize;
            else if (!altShiftSize.isEmpty())
                defaultSize = altShiftSize;
        }
        if (defaultSize.isEmpty() && altSize.isEmpty()) {
            if (!shiftSize.isEmpty())
                defaultSize = shiftSize;
            else if (!altShiftSize.isEmpty())
                defaultSize = altShiftSize;
        }
        if (altShiftSize.isEmpty() && altSize.isEmpty()) {
            if (!defaultSize.isEmpty())
                altShiftSize = defaultSize;
            else if (!shiftSize.isEmpty())
                altShiftSize = shiftSize;
        }
        if (altShiftSize.isEmpty() && shiftSize.isEmpty()) {
            if (!defaultSize.isEmpty())
                altShiftSize = defaultSize;
            else if (!altSize.isEmpty())
                altShiftSize = altSize;
        }

    } else if (d->layoutHint == EconomicLayoutHint) {
        // Nothing to do here because economic means smallest possible size hint
    }

    // Compute width of the widget
    size.setWidth(size.width() + qMax(defaultSize.width(), shiftSize.width()));
    if (!altSize.isEmpty() || !altShiftSize.isEmpty())
        size.setWidth(size.width() + d->spacingHorizontal);
    size.setWidth(size.width() + qMax(altSize.width(), altShiftSize.width()));

    // Compute height of the widget
    size.setHeight(size.height() + qMax(defaultSize.height(), altSize.height()));
    if (!shiftSize.isEmpty() || !altShiftSize.isEmpty())
        size.setHeight(size.height() + d->spacingVertical);
    size.setHeight(size.height() + qMax(shiftSize.height(), altShiftSize.height()));

    return size;
}

/*! \reimp */
QSize QVirtualKey::minimumSizeHint() const
{
    return sizeHint();
}

/*! \reimp */
bool QVirtualKey::event(QEvent *event)
{
    return QAbstractButton::event(event);
}

/*! \reimp */
void QVirtualKey::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    QStyleOptionButton button;
    button.initFrom(this);
    button.icon = icon();
    button.iconSize = iconSize();
    if (isDown())
        button.state |= QStyle::State_Sunken;
    if (isChecked())
        button.state |= QStyle::State_On;
    if (!isChecked() && !isDown())
        button.state |= QStyle::State_Raised;

    QRect rect = style()->subElementRect(QStyle::SE_PushButtonContents, &button, this);
    QStyle::State bflags = button.state;

    QStyleOption tool(0);
    if (bflags & (QStyle::State_Sunken | QStyle::State_On | QStyle::State_Raised)) {
        tool.rect = this->rect();
        tool.state = bflags;
        tool.palette = button.palette;
        tool.palette.setBrush(QPalette::Button, d->backgroundBrush);
        tool.palette.setBrush(QPalette::Window, d->backgroundBrush);
        style()->drawPrimitive(QStyle::PE_PanelButtonTool, &tool, &painter, this);
    }

    if (button.state & QStyle::State_HasFocus) {
        QStyleOptionFocusRect fr;
        fr.QStyleOption::operator=(button);
        fr.rect.adjust(3, 3, -3, -3);
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &fr, &painter, this);
    }

    if (button.state & (QStyle::State_On | QStyle::State_Sunken))
        rect.translate(style()->pixelMetric(QStyle::PM_ButtonShiftHorizontal, &button, this),
                       style()->pixelMetric(QStyle::PM_ButtonShiftVertical, &button, this));

    rect.adjust(d->spacingHorizontal, d->spacingVertical, -d->spacingHorizontal, -d->spacingVertical);
    QRect defaultRect = rect;
    QRect shiftRect = rect;
    QRect altRect = rect;
    QRect altShiftRect = rect;
    uint tf = d->alignmentHint | Qt::TextShowMnemonic;
    if (!style()->styleHint(QStyle::SH_UnderlineShortcut, &button, this))
        tf |= Qt::TextHideMnemonic;

    // The layout hint controls wether the widget resembles full keyboard key like
    // widget to have a consisten look if not all keys have all or different (default,
    // shift, alt or alt-shift) key bindings or wether it is economically layouted
    // using only the space it really needs

    if (d->layoutHint == DefaultLayoutHint) {
        defaultRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);
        defaultRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);

        shiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
        shiftRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);

        altRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);
        altRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);

        altShiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
        altShiftRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);

    } else if (d->layoutHint == EconomicLayoutHint) {
        if (!d->altShiftText.isNull() || !d->altShiftIcon.isNull()) {
            defaultRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);
            defaultRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);
        } else {
            if (!d->altText.isNull() || !d->altIcon.isNull())
                defaultRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);
            if (!d->shiftText.isNull() || !d->shiftIcon.isNull())
                defaultRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);
        }

        if (!d->altText.isNull() || !d->altIcon.isNull()) {
            shiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
            shiftRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);
        } else {
            if (!d->altShiftText.isNull() || !d->altShiftIcon.isNull())
                shiftRect.setRight(rect.left() + rect.width() / 2 - d->spacingHorizontal / 2);
            if (!text().isNull() || !icon().isNull())
                shiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
        }

        if (!d->shiftText.isNull() || !d->shiftIcon.isNull()) {
            altRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);
            altRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);
        } else {
            if (!d->altShiftText.isNull() || !d->altShiftIcon.isNull())
                altRect.setTop(rect.top() + rect.height() / 2 + d->spacingVertical / 2);
            if (!text().isNull() || !icon().isNull())
                altRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);
        }

        if (!text().isNull() || !icon().isNull()) {
            altShiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
            altShiftRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);
        } else {
            if (!d->shiftText.isNull() || !d->shiftIcon.isNull())
                altShiftRect.setLeft(rect.left() + rect.width() / 2 + d->spacingHorizontal / 2);
            if (!d->altText.isNull() || !d->altIcon.isNull())
                altShiftRect.setBottom(rect.top() + rect.height() / 2 - d->spacingVertical / 2);
        }
    }

    // Draw all key binding
    paintSubElement(&painter, shiftText(), shiftIcon(), shiftRect, tf, button.state);
    paintSubElement(&painter, altText(), altIcon(), altRect, tf, button.state);
    paintSubElement(&painter, text(), icon(), defaultRect, tf, button.state);
    paintSubElement(&painter, altShiftText(), altShiftIcon(), altShiftRect, tf, button.state);
}

/*!
    \brief This helper method is used to draw the subelements of a key.
*/
void QVirtualKey::paintSubElement(QPainter *painter, const QString &text, const QIcon &icon, const QRect &rect, uint tf, QStyle::State state)
{
    if (icon.isNull()) {        // Draw text if no icon present
        style()->drawItemText(painter, rect, tf, palette(), (state & QStyle::State_Enabled), text, QPalette::ButtonText);
    } else {                    // Draw icon, always preceedes text
        QIcon::Mode mode = state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
        if (mode == QIcon::Normal && state & QStyle::State_HasFocus)
            mode = QIcon::Active;
        QIcon::State st = QIcon::Off;
        if (state & QStyle::State_On)
            st = QIcon::On;

        QPixmap pixmap = icon.pixmap(iconSize(), mode, st);
        style()->drawItemPixmap(painter, rect, tf, pixmap);
    }
}
