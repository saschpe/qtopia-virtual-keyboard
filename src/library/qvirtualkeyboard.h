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

#ifndef QVIRTUALKEYBOARD_H
#define QVIRTUALKEYBOARD_H

#include <QObject>
#include <QKeyEvent>

#include "qvirtualkeyboardglobal.h"

class QVirtualKey;

class QVirtualKeyboardPrivate;

class Q_QVK_EXPORT QVirtualKeyboard : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Qt::Key shiftModifier READ shiftModifier WRITE setShiftModifier)
    Q_PROPERTY(Qt::Key altModifier READ altModifier WRITE setAltModifier)
    Q_PROPERTY(bool autoShifting READ autoShifting WRITE setAutoShifting)
    Q_PROPERTY(bool deadKeys READ deadKeys WRITE setDeadKeys)
    Q_PROPERTY(bool capsLock READ capsLock WRITE setCapsLock)

public:
    explicit QVirtualKeyboard(QObject *parent = 0);
    virtual ~QVirtualKeyboard();

    bool addKeyContainer(QObject *object);
    void removeKeyContainer(QObject *object);

    void setShiftModifier(Qt::Key modifierKey);
    Qt::Key shiftModifier() const;
    void setAltModifier(Qt::Key modifierKey);
    Qt::Key altModifier() const;

    void setAutoShifting(bool enabled);
    bool autoShifting() const;

    void setDeadKeys(bool enabled);
    bool deadKeys() const;
    static bool isDeadKey(Qt::Key key);

    void setCapsLock(bool enabled);
    bool capsLock() const;

    bool setLayout(const QString &fileName);
    void setLayoutVersion(int version);
    int layoutVersion() const;
    void setLayoutName(const QString &name);
    const QString layoutName() const;

    QVirtualKey *findVirtualKey(const QString &name) const;
    static Qt::Key stringToKey(const QString &string);

Q_SIGNALS:
    void keyEvent(QKeyEvent *);
    void keyPressed(int key, Qt::KeyboardModifiers modifiers, const QString &text);
    void keyReleased(int key, Qt::KeyboardModifiers modifiers, const QString &text);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    QKeyEvent *generateKeyEvent(const QVirtualKey &vk, QKeyEvent::Type type);

private:
    static Qt::KeyboardModifier keyToKeyboardModifier(Qt::Key key);
    static Qt::Key deadKeyToUndeadKey(Qt::Key key);
    static Qt::Key combineKeys(Qt::Key key1, Qt::Key key2);

    QVirtualKeyboardPrivate *d;

    friend class QVirtualKeyboardLayoutReader;
};

#endif
