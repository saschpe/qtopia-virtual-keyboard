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

#include "qvirtualkeyboard.h"
#include "qvirtualkey.h"
#include "qvirtualkeyboardlayoutreader.h"

#include <QEvent>
#include <QChildEvent>
#include <QFile>
#include <QMetaEnum>
#include <QDebug>

#include "qvirtualkeyboard_p.h"

/*!
    \class QVirtualKeyboard qvirtualkeyboard.h
    \brief The virtual keyboard acts as a collector for virtual keys and sends events to
           receiver objects.
    \mainclass

    By registering an object to the virtual keyboard all virtual key children of
    the object are monitored by the virtual keyboard. The virtual keyboard also tracks
    addition and removal of childrens of 'object'. This also implies that key events are
    generated just for registered virtual keys. The key events are sent to all registered
    receivers. It is possible to use as many virtual keyboards (with several virtual
    keys used by different keyboards).

    All virtual key press and release events are handled by the virtual keyboard, direct
    interaction with the virtual keys is undesired and thus impossible.

    \sa QVirtualKey, QKeyEvent
*/

/*!
    \property QVirtualKeyboard::shiftModifier
    \brief This modifier triggers the shifting behavior of virtual keys.

    This means if a virtual key is pressed while a shift modifier key is pressed, it
    generates the keycode which is configured as 'shiftKey' (up left on the virtual key).
    When combined with the alt modifier, the 'altShiftKey' keycode is generated (up right).

    This property is set to Qt::Key_Shift by default.

    \sa Qt::Key
*/

/*!
    \property QVirtualKeyboard::altModifier
    \brief This modifier triggers the alternative behavior of virtual keys.

    This means if a virtual key is pressed while a alternative modifier key is pressed, it
    generates the keycode which is configured as 'altKey' (down right on the virtual key).
    When combined with the shift modifier, the 'altShiftKey' keycode is generated (up right).

    This property is set to Qt::Key_AltGr by default.

    \sa Qt::Key
*/

/*!
    \property QVirtualKeyboard::autoShifting
    \brief Controls wether character-only keys are automatically shifted or not.

    Full-layout keyboards usually have character keys, which react to the shift key by
    generating an uppercase character and otherwise a lowercase one . You can mimic this
    behavior for virtual keys which have only the 'key' property set to TAB, SPACE or
    a character. So instead of having keys configured like:

    \code
        ___
       | A |
       | a |
        ---
    \endcode

    You can just create a key with a character as 'key' and set auto-shifting to 'true'
    to get the same result:

    \code
        ___
       |   |
       | A |
        ---
    \endcode

    This property is enabled by default.
*/

/*!
    \property QVirtualKeyboard::deadKeys
    \brief Configures if dead keys react on other keys or if they are ignored.

    If your keyboard layout contains dead keys and you want them to modify keys
    set this property to 'true'.

    This property is enabled by default.
*/

/*!
    \property QVirtualKeyboard::capsLock
    \brief Controles if capslock influences the 'shift' modifier or not.

    If this property is set to 'true' and the user presses a virtual key which
    generates Qt::Key_CapsLock the 'shift' modifier is enabled until the user
    releases (or unchecks) the virtual key.

    This property is enabled by default.
*/

/*!
    \brief Construct a virtual keyboard with no registered keys and a \a parent.
*/
QVirtualKeyboard::QVirtualKeyboard(QObject *parent)
    : QObject(parent)
    , d(new QVirtualKeyboardPrivate)
{
}

/*!
    \brief Unregisters all remaining container and receiver objects.
*/
QVirtualKeyboard::~QVirtualKeyboard()
{
    foreach (QObject *object, d->virtualKeyHash.keys())
        removeKeyContainer(object);

    delete d;
}

/*!
    \brief Register an \a object which contains virtual keys to monitor for key events.

    The object is also monitored for child events. This means if a virtual key
    is added to or removed from the object, the virtual keyboard automatically handles that.
*/
bool QVirtualKeyboard::addKeyContainer(QObject *object)
{
    if (!object) {
        qWarning("QVirtualKeyboard::addKeyContainer(): Attempt to register a null object");
        return false;
    }

    // Grab all QVirtualKey children of 'object' to handle their events
    QList<QVirtualKey *> keys = d->virtualKeyHash.value(object);
    foreach (QVirtualKey *newKey, object->findChildren<QVirtualKey *>()) {
        //FIXME: Check if newKey already has 'this' as eventFilter, which could happen
        //       when the user registers first a container and then it's parent container
        if (!keys.contains(newKey)) {
            newKey->installEventFilter(this);
            keys.append(newKey);
        }
    }
    d->virtualKeyHash.insert(object, keys);

    // Watch the object for added/removed child objects which could be virtual keys.
    // This also applies for the case the the container is actually a virtual key.
    object->installEventFilter(this);
    return true;
}

/*!
    \brief Unregister an \a object previously monitored.
*/
void QVirtualKeyboard::removeKeyContainer(QObject *object)
{
    if (!object) {
        qWarning("QVirtualKeyboard::removeKeyContainer(): Attempt to unregister a null object");
        return;
    }

    // Remove event filter from all watched virtual keys and unregister the container
    if (d->virtualKeyHash.contains(object)) {
        object->removeEventFilter(this);
        foreach (QVirtualKey *key,  d->virtualKeyHash.value(object))
            key->removeEventFilter(this);
        d->virtualKeyHash.remove(object);
    }
}

/*!
    \brief Configure which key (modifier) is used to trigger the shifted virtual key code.

    You can basically use every key except the key which is already set as alternative
    modifier.

    \sa QVirtualKey
*/
void QVirtualKeyboard::setShiftModifier(Qt::Key modifierKey)
{
    d->shiftModifier = modifierKey;
}

/*!
    \brief Returns the 'shift' modifier value.
*/
Qt::Key QVirtualKeyboard::shiftModifier() const
{
    return d->shiftModifier;
}

/*!
    \brief Configure which key (modifier) is used to trigger the alternative virtual key code.

    You can basically use every key except the key which is already set as shift
    modifier.

    \sa altModifier, altModifier()
*/
void QVirtualKeyboard::setAltModifier(Qt::Key modifierKey)
{
    d->altModifier = modifierKey;
}

/*!
    \brief Returns the current \i altModifier.

    \sa altModifier, setAltModifier()
*/
Qt::Key QVirtualKeyboard::altModifier() const
{
    return d->altModifier;
}

/*!
    \brief Converts a normal Qt::Key to the corresponding Qt::KeyboardModier.

    \sa Qt::KeyboardModifier, Qt::Key
*/
Qt::KeyboardModifier QVirtualKeyboard::keyToKeyboardModifier(Qt::Key key)
{
    switch (key) {
        case Qt::Key_Shift: return Qt::ShiftModifier;
        case Qt::Key_Control: return Qt::ControlModifier;
        case Qt::Key_Alt: return Qt::AltModifier;
        case Qt::Key_Meta: return Qt::MetaModifier;
        default: return Qt::NoModifier;
    }
}

/*!
    \brief Set wether character keys are shifted automatically.

    \sa autoShifting, autoShifting()
*/
void QVirtualKeyboard::setAutoShifting(bool enabled)
{
    d->autoShifting= enabled;
}

/*!
    \brief Returns the auto-shifting behavior setting.

    \sa autoShifting, setAutoShifting()
*/
bool QVirtualKeyboard::autoShifting() const
{
    return d->autoShifting;
}

/*!
    \brief Set dead key behavior.

    \sa deadKeys, deadKeys()
*/
void QVirtualKeyboard::setDeadKeys(bool enabled)
{
    d->deadKeys= enabled;
}

/*!
    \brief Returns the dead-keys behavior setting.

    \sa deadKeys, setDeadKeys()
*/
bool QVirtualKeyboard::deadKeys() const
{
    return d->deadKeys;
}

/*!
    \brief Checks wether the given \a key is a dead key.
*/
bool QVirtualKeyboard::isDeadKey(Qt::Key key)
{
    switch (key) {
        case Qt::Key_Dead_Grave:
        case Qt::Key_Dead_Acute:
        case Qt::Key_Dead_Circumflex:
        case Qt::Key_Dead_Tilde:
        case Qt::Key_Dead_Macron:
        case Qt::Key_Dead_Breve:
        case Qt::Key_Dead_Abovedot:
        case Qt::Key_Dead_Diaeresis:
        case Qt::Key_Dead_Abovering:
        case Qt::Key_Dead_Doubleacute:
        case Qt::Key_Dead_Caron:
        case Qt::Key_Dead_Cedilla:
        case Qt::Key_Dead_Ogonek:
        case Qt::Key_Dead_Iota:
        case Qt::Key_Dead_Voiced_Sound:
        case Qt::Key_Dead_Semivoiced_Sound:
        case Qt::Key_Dead_Belowdot:
        case Qt::Key_Dead_Hook:
        case Qt::Key_Dead_Horn: return true;
        default: return false;
    }
}

/*!
    \brief Converts a dead key into the undead equivalent if possible.

    Not all dead keys have an "undead" representation (for example the
    \a cedil makes only sense together with \a C to be the \a &Ccedil;

    \sa deadKeys, Qt::Key
*/
Qt::Key QVirtualKeyboard::deadKeyToUndeadKey(Qt::Key key)
{
    Qt::Key ret = Qt::Key_unknown;
    switch (key) {
        case Qt::Key_Dead_Grave: break;
        case Qt::Key_Dead_Acute: ret = Qt::Key_acute; break;
        case Qt::Key_Dead_Circumflex: ret = Qt::Key_AsciiCircum; break;
        case Qt::Key_Dead_Tilde: ret = Qt::Key_AsciiTilde; break;
        case Qt::Key_Dead_Macron: ret = Qt::Key_macron; break;
        case Qt::Key_Dead_Breve: break;
        case Qt::Key_Dead_Abovedot: break;
        case Qt::Key_Dead_Diaeresis: ret = Qt::Key_diaeresis; break;
        case Qt::Key_Dead_Abovering: break;
        case Qt::Key_Dead_Doubleacute: break;
        case Qt::Key_Dead_Caron: break;
        case Qt::Key_Dead_Cedilla: ret = Qt::Key_cedilla; break;
        case Qt::Key_Dead_Ogonek: break;
        case Qt::Key_Dead_Iota: break;
        case Qt::Key_Dead_Voiced_Sound: break;
        case Qt::Key_Dead_Semivoiced_Sound: break;
        case Qt::Key_Dead_Belowdot: break;
        case Qt::Key_Dead_Hook: break;
        case Qt::Key_Dead_Horn: break;
        default: break; // To avoid compile-time errors
    }
    return ret;
}

/*!
    \brief Combines two distinct keys into one if possible.

    This is useful if the user pressed a dead key and afterwards a key
    which he wants to be combined with the former.

    \sa deadKeys, Qt::Key
*/
Qt::Key QVirtualKeyboard::combineKeys(Qt::Key key1, Qt::Key key2)
{
    // Sanity check, we can not combine two dead keys or two normal keys
    // Note: The latter might change in the future if useful cases are found.
    if (isDeadKey(key1) && isDeadKey(key2) || !isDeadKey(key1) && !isDeadKey(key2))
        return Qt::Key_unknown;

    Qt::Key deadKey = isDeadKey(key1) ? key1 : key2;
    Qt::Key normKey = isDeadKey(key1) ? key2 : key1;
    Qt::Key ret = Qt::Key_unknown;

    switch (deadKey) {
        case Qt::Key_Dead_Grave:
            switch (normKey) {
                case Qt::Key_A: ret = Qt::Key_Agrave; break;
                case Qt::Key_E: ret = Qt::Key_Egrave; break;
                case Qt::Key_I: ret = Qt::Key_Igrave; break;
                case Qt::Key_O: ret = Qt::Key_Ograve; break;
                case Qt::Key_U: ret = Qt::Key_Ugrave; break;
                default: break; // To avoid compile-time errors
            }
            break;
        case Qt::Key_Dead_Acute:
            switch (normKey) {
                case Qt::Key_A: ret = Qt::Key_Aacute; break;
                case Qt::Key_E: ret = Qt::Key_Eacute; break;
                case Qt::Key_I: ret = Qt::Key_Iacute; break;
                case Qt::Key_O: ret = Qt::Key_Oacute; break;
                case Qt::Key_U: ret = Qt::Key_Uacute; break;
                case Qt::Key_Y: ret = Qt::Key_Yacute; break;
                default: break; // To avoid compile-time errors
            }
            break;
        case Qt::Key_Dead_Circumflex:
            switch (normKey) {
                case Qt::Key_A: ret = Qt::Key_Acircumflex; break;
                case Qt::Key_E: ret = Qt::Key_Ecircumflex; break;
                case Qt::Key_I: ret = Qt::Key_Icircumflex; break;
                case Qt::Key_O: ret = Qt::Key_Ocircumflex; break;
                case Qt::Key_U: ret = Qt::Key_Ucircumflex; break;
                default: break; // To avoid compile-time errors
            }
            break;
        case Qt::Key_Dead_Tilde:
            switch (normKey) {
                case Qt::Key_A: ret = Qt::Key_Atilde; break;
                case Qt::Key_N: ret = Qt::Key_Ntilde; break;
                case Qt::Key_O: ret = Qt::Key_Otilde; break;
                default: break; // To avoid compile-time errors
            }
            break;
        case Qt::Key_Dead_Macron:
            break;
        case Qt::Key_Dead_Breve:
            break;
        case Qt::Key_Dead_Abovedot:
            break;
        case Qt::Key_Dead_Diaeresis:
            switch (normKey) {
                case Qt::Key_A: ret = Qt::Key_Adiaeresis; break;
                case Qt::Key_E: ret = Qt::Key_Ediaeresis; break;
                case Qt::Key_I: ret = Qt::Key_Idiaeresis; break;
                case Qt::Key_O: ret = Qt::Key_Odiaeresis; break;
                case Qt::Key_U: ret = Qt::Key_Udiaeresis; break;
                case Qt::Key_Y: ret = Qt::Key_ydiaeresis; break;
                default: break; // To avoid compile-time errors
            }
            break;
        case Qt::Key_Dead_Abovering:
            if (normKey == Qt::Key_A)
                ret = Qt::Key_Aring;
            break;
        case Qt::Key_Dead_Doubleacute:
            break;
        case Qt::Key_Dead_Caron:
            break;
        case Qt::Key_Dead_Cedilla:
            if (normKey == Qt::Key_C)
                ret = Qt::Key_Ccedilla;
            break;
        case Qt::Key_Dead_Ogonek:
            break;
        case Qt::Key_Dead_Iota:
            break;
        case Qt::Key_Dead_Voiced_Sound:
            break;
        case Qt::Key_Dead_Semivoiced_Sound:
            break;
        case Qt::Key_Dead_Belowdot:
            break;
        case Qt::Key_Dead_Hook:
            break;
        case Qt::Key_Dead_Horn:
            break;
        default: break; // To avoid compile-time errors
    }
    return ret;
}

/*!
    \brief Set capslock behavior.

    \sa capsLock, capsLock()
*/
void QVirtualKeyboard::setCapsLock(bool enabled)
{
    d->capsLock = enabled;
}

/*!
    \brief Returns the state of capslock behavior.

    \sa capsLock, setCapsLock()
*/
bool QVirtualKeyboard::capsLock() const
{
    return d->capsLock;
}

/*!
    \brief Loads a new keyboard layout and changes it.

    A virtual keyboard layout XML file is parsed and the virtual
    keyboard tries to match and update all registered virtual keys that
    are found in the layout file. If a virtual key is not described in
    the layout XML file with \a fileName it won't be changed at all.
    Non-critical errors in the XML file (wrong structure, unknown
    elements, ...) are gently ignored with a warning printed on stdout;

    \sa QVirtualKeyboardLayoutReader
*/
bool QVirtualKeyboard::setLayout(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QVirtualKeyboardLayoutReader reader(this);
        if (!reader.read(&file)) {
            qWarning() << "QVirtualKeyboard::setLayout(" << fileName << ")" << reader.errorString();
            return false;
        } else
            return true;
    } else {
        qWarning() << "QVirtualKeyboard::setLayout(" << fileName << ") Unable to find keyboard layout!";
        return false;
    }
}

/*!
    \brief Set the current keyboard layout \a version.

    This is just informational and can be used by GUI's.
*/
void QVirtualKeyboard::setLayoutVersion(int version)
{
    d->keyboardLayoutVersion = version;
}

/*!
    \brief Retrieve the current keyboard layout version number.

    This is just informational and can be used by GUI's.
*/
int QVirtualKeyboard::layoutVersion() const
{
    return d->keyboardLayoutVersion;
}

/*!
    \brief Set the current keyboard layout \a name.

    This is just informational and can be used by GUI's.
*/
void QVirtualKeyboard::setLayoutName(const QString &name)
{
    d->keyboardLayoutName = name;
}

/*!
    \brief Retrieve the current keyboard layout name.

    This is just informational and can be used by GUI's.
*/
const QString QVirtualKeyboard::layoutName() const
{
    return d->keyboardLayoutName;
}

/*!
    \brief Find a virtual key child object with \a name within all key
           containers registered with this virtual keyboard.

    \sa QObject::findChild
*/
QVirtualKey *QVirtualKeyboard::findVirtualKey(const QString &name) const
{
    foreach (QObject *object, d->virtualKeyHash.keys()) {
        QVirtualKey *key = object->findChild<QVirtualKey *>(name);
        if (key)
            return key;
    }
    qWarning() << "QVirtualKeyboard::findVirtualKey(" << name << ") Unable to find virtual key!";
    return 0;
}

/*!
    \brief Tries to convert \a string describing a key to a Qt::Key.
*/
Qt::Key QVirtualKeyboard::stringToKey(const QString &string)
{
    // Qt's magic in undocumented features rocks. Find the QMetaEnum of Qt::Key to
    // easily get the enum value of a string representation that value.
    int index =  QObject::staticQtMetaObject.indexOfEnumerator("Key");
    QMetaEnum me = QObject::staticQtMetaObject.enumerator(index);
    int key = me.keyToValue(string.toLatin1());

    if (key >= 0) {
        return (Qt::Key)key;
    } else {
        qWarning() << "QVirtualKeyboard::stringToKey() Unable to convert" << string << "to key, using Qt::Key_unknown";
        return Qt::Key_unknown;
    }
}

/*!
    \reimp
    \brief This event filter is transparent, some events are modified but none are filtered
*/
bool QVirtualKeyboard::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent *ce = static_cast<QChildEvent *>(event);
        // Install event filter for added virtual key children
        if(qobject_cast<QVirtualKey *>(ce->child()))
            ce->child()->installEventFilter(this);

    } else if (event->type() == QEvent::ChildRemoved) {
        QChildEvent *ce = static_cast<QChildEvent *>(event);
        // Remove event filter from removed virtual key children
        if(qobject_cast<QVirtualKey *>(ce->child()))
            ce->child()->removeEventFilter(this);

    } else if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick
            || event->type() == QEvent::KeyPress) {
        QVirtualKey *vk = qobject_cast<QVirtualKey *>(object);
        if (vk) {
            // The user pressed a virtual key, generate key event and send to all receivers.
            QKeyEvent::Type keyEventType;
            if (vk->isCheckable())
                vk->isChecked() ? keyEventType = QKeyEvent::KeyRelease : keyEventType = QKeyEvent::KeyPress;
            else
                keyEventType = QKeyEvent::KeyPress;
            QKeyEvent *event = generateKeyEvent(*vk, keyEventType);
            //qDebug() << "QVirtualKeyboard::eventFilter() Received press event, send " << event;

            emit keyEvent(event);
            emit keyPressed(event->key(), event->modifiers(), event->text());
        }

    } else if (event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::KeyRelease) {
        QVirtualKey *vk = qobject_cast<QVirtualKey *>(object);
        if (vk && !vk->isCheckable()) {
            // The user released a virtual key, generate key event and send to all receivers.
            // Checkable keys get their key release event when you klick (keypress) it to release it
            QKeyEvent *event = generateKeyEvent(*vk, QKeyEvent::KeyRelease);
            //qDebug() << "QVirtualKeyboard::eventFilter() Received release event, send" << event;

            emit keyEvent(event);
            emit keyReleased(event->key(), event->modifiers(), event->text());
        }
    }
    return false;
}

/*!
    \brief Helper method to generate a QKeyEvent based on the provided virtual key \a vk
           and \a type of user input.
*/
QKeyEvent *QVirtualKeyboard::generateKeyEvent(const QVirtualKey &vk, QKeyEvent::Type type)
{
    Q_ASSERT(type == QKeyEvent::KeyPress || type == QKeyEvent::KeyRelease);

    Qt::Key key(Qt::Key_unknown);
    Qt::KeyboardModifiers modifiers(Qt::NoModifier);

    // If the pressed key is one of our designated modifiers, we put them into
    // a special hash where we count how often it was pressed (use case: two
    // shift keys, one is pressed, one is released, ...).
    if (vk.key() == d->shiftModifier || vk.key() == d->altModifier) {
        if (type == QKeyEvent::KeyPress) {
            if (d->currentModifierHash.contains(vk.key()))
                d->currentModifierHash.insert(vk.key(), 1);
            else
                d->currentModifierHash.insert(vk.key(), d->currentModifierHash.value(vk.key()) + 1);
        } else {
            d->currentModifierHash.insert(vk.key(), d->currentModifierHash.value(vk.key()) - 1);
        }
        key = Qt::Key_unknown;
    } else if (d->currentModifierHash.value(d->shiftModifier) > 0 && d->currentModifierHash.value(d->altModifier) > 0) {
        // Shift and 'altModifier' are both pressed
        key = vk.altShiftKey();
        // The exakt nature of the modifiers is unimportant for us to generate the corresponding
        // key event, but if it's a common one, we send it to not confuse the receiving QObject too much.
        modifiers |= keyToKeyboardModifier(d->altModifier);
        modifiers |= keyToKeyboardModifier(d->shiftModifier);
    } else if (d->currentModifierHash.value(d->shiftModifier) > 0) {
        // Only shift is pressed, if auto-shifting is enabled, use the default key value and remember it
        // for proper later key unicode generation.
        if (d->autoShifting && vk.shiftKey() == Qt::Key_unknown) {
            key = vk.key();
            d->autoShiftingMark = true;
        } else {
            key = vk.shiftKey();
        }
        modifiers |= keyToKeyboardModifier(d->shiftModifier);
    } else if (d->currentModifierHash.value(d->altModifier) > 0) {
        // Only 'altModifier' is pressed
        key = vk.altKey();
        modifiers |= keyToKeyboardModifier(d->altModifier);
    } else {
        // No modifiers are pressed
        key = vk.key();
        modifiers = Qt::NoModifier;
    }

    // Altough we already considered the currently pressed modifiers, we only checked for
    // the configured 'shift' modifier and the configured 'alt' modifier, which can be
    // arbitrary keys. It can thus happen, that the user has a virtual key with the
    // Qt::Key_Control keycode set, it is checked but not registered as 'alt' or
    // 'shift' modifier. The intention then is to send Qt::ControlModifier away with the
    // generated QKeyEvent to mimic what happens with every standard physical keyboard.
    // So if Qt::Key_Control is pressed, we should remember this until it is released and
    // apply it to the currently set modifiers. Long story, eh?
    if (type == QEvent::KeyPress) {
        d->rememberedStandardModifiers |= keyToKeyboardModifier(key);
    } else {
        d->rememberedStandardModifiers &= ~keyToKeyboardModifier(key);
    }

    // CapsLock functionality is similar to modifier handling above.
    if (d->capsLock && key == Qt::Key_CapsLock) {
        if (type == QKeyEvent::KeyPress) {
            if (!d->currentModifierHash.contains(d->shiftModifier))
                d->currentModifierHash.insert(d->shiftModifier, 1);
            else
                d->currentModifierHash.insert(d->shiftModifier, d->currentModifierHash.value(d->shiftModifier) + 1);
        } else {
            d->currentModifierHash.insert(d->shiftModifier, d->currentModifierHash.value(d->shiftModifier) - 1);
        }
    }

    // If dead key behavior is on and we have a dead key do special threatment
    if (d->deadKeys && type == QEvent::KeyPress) {
        if (isDeadKey(key)) {
            if (d->lastDeadKey == key) {
                key = deadKeyToUndeadKey(key);
                d->lastDeadKey = Qt::Key_unknown;
            } else {
                d->lastDeadKey = key;
                key = Qt::Key_unknown;
            }
        } else if (d->lastDeadKey != Qt::Key_unknown && key != Qt::Key_unknown) {
            key = combineKeys(d->lastDeadKey, key);
            d->lastDeadKey = Qt::Key_unknown;
        }
    }

    // Generate unicode to send together with the key and apply some further
    // fine-tuning for some keys with unpleasant behavior.
    QString unicode = "";
    switch (key) {
        case Qt::Key_unknown:
        case Qt::Key_Shift:
        case Qt::Key_Control:
        case Qt::Key_Alt:
        case Qt::Key_Meta:
        case Qt::Key_Super_L:
        case Qt::Key_Super_R:
        case Qt::Key_Menu:
        case Qt::Key_CapsLock:
        case Qt::Key_NumLock:
        case Qt::Key_Escape: break;
        //NOTE: Maybe there are some more keys out there which should not generate
        //      any visible output
        case Qt::Key_Tab: unicode = "\t"; break;
        default: unicode = QString(key);
    }

    // Last but not least check if auto-shifting is enabled, we generate a lowercase
    // unicode value, otherwise auto-shift was applied (because the user pressed the
    // 'shift' modifier key), we send it uppercase. This mimics the behavior of full
    // keyboard layouts.
    if (d->autoShifting) {
        d->autoShiftingMark ? unicode = unicode.toUpper() : unicode = unicode.toLower();
        d->autoShiftingMark = false;
    }
    return new QKeyEvent(type, key, modifiers | d->rememberedStandardModifiers, unicode, vk.autoRepeat());
}
