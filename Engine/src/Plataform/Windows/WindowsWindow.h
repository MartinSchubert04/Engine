#include "Core/Window.h"
#include "GLFW/glfw3.h"

namespace Engine {

class WindowsWindow : public Window {

public:
  WindowsWindow(const WindowProps &props);
  virtual ~WindowsWindow();

  void onUpdate() override;

  uint32_t getWidth() const override;
  uint32_t getHeight() const override;

  // Window attributes
  void setEventCallback(const EventCallbackFn &callback) override;
  void setVSync(bool enabled) override;
  bool isVSync() const override { return mData.vsync; };

  void *getNativeWindow() const override { return mWindow; };

private:
  virtual void init(const WindowProps &props);
  virtual void shutdown();

private:
  GLFWwindow *mWindow;

  struct WindowData {
    std::string title;
    unsigned int width, height;

    bool vsync;

    EventCallbackFn EventCallback;
  };

  WindowData mData;
};

}  // namespace Engine
