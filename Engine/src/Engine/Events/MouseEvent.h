#include "Event.h"
#include "MouseCodes.h"
namespace Engine {

class MouseMovedEvent : public Event {

public:
  MouseMovedEvent(float posX, float posY) : mPosX(posX), mPosY(posY) {}

  inline float getX() const { return mPosX; }
  inline float getY() const { return mPosY; }

  std::string toString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << getX() << ", " << getX();
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
  float mPosX, mPosY;
};

class MouseScrolledEvent : public Event {

public:
  MouseScrolledEvent(float offsetX, float offsetY) : mOffsetX(offsetX), mOffsetY(offsetY) {}

  inline float getOffsetX() const { return mOffsetX; }
  inline float getOffsetY() const { return mOffsetY; }

  std::string toString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << getOffsetX() << ", " << getOffsetY();
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
  float mOffsetX, mOffsetY;
};

class MouseButtonEvent : public Event {

public:
  MouseCode GetMouseButton() const { return mButton; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
protected:
  MouseButtonEvent(const MouseCode button) : mButton(button) {}

  MouseCode mButton;
};

class MouseButtonPressedEvent : public MouseButtonEvent {

public:
  MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string toString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << mButton;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {

  MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string toString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << mButton;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};

}  // namespace Engine
