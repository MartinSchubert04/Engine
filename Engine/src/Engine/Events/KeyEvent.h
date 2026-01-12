#include "Event.h"
#include "KeyCodes.h"
namespace Engine {

class KeyEvent : public Event {

public:
  KeyCode getKeyCode() const { return mKeyCode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
  KeyEvent(const KeyCode keycode) : mKeyCode(keycode) {};

  KeyCode mKeyCode;
};

class KeyPressedEvent : public KeyEvent {

public:
  KeyPressedEvent(KeyCode keycode, bool isRepeat = false) : KeyEvent(keycode), mIsRepeat(isRepeat) {}

  bool isRepeat() { return mIsRepeat; }

  std::string toString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << mKeyCode << " (repeat = " << mIsRepeat << ")";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

private:
  bool mIsRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string toString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << mKeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
  KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string toString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << mKeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};

}  // namespace Engine
