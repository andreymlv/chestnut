#include <glad/glad.h>

#include <glfwpp/glfwpp.h>

#include <iostream>

int main() {
  auto GLFW = glfw::init();

  glfw::WindowHints hints;
  hints.clientApi = glfw::ClientApi::OpenGl;
  hints.contextVersionMajor = 4;
  hints.contextVersionMinor = 6;
#ifdef __APPLE__
  hints.openglForwardCompat = true;
#endif
  hints.apply();

  glfw::Window window{640, 480, "Hello GLFWPP"};
  glfw::makeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  window.framebufferSizeEvent.setCallback(
      [](glfw::Window &, int width, int height) {
        glViewport(0, 0, width, height);
      });

  window.keyEvent.setCallback([&window](glfw::Window &, glfw::KeyCode keyCode,
                                        int scanCode, glfw::KeyState state,
                                        glfw::ModifierKeyBit modifiers) {
    float val = 0.5;
    if (modifiers & glfw::ModifierKeyBit::Control) {
      val += 0.25;
    }
    if (modifiers & glfw::ModifierKeyBit::Shift) {
      val -= 0.25;
    }
    switch (keyCode) {
    case glfw::KeyCode::Escape:
      window.setShouldClose(true);
      break;
    case glfw::KeyCode::R:
      glClearColor(val, 0.0, 0.0, val);
      break;
    case glfw::KeyCode::G:
      glClearColor(0.0, val, 0.0, val);
      break;
    case glfw::KeyCode::B:
      glClearColor(0.0, 0.0, val, val);
      break;
    case glfw::KeyCode::M:
      glClearColor(val, 0.0, val, val);
      break;
    case glfw::KeyCode::C:
      glClearColor(0.0, val, val, val);
      break;
    case glfw::KeyCode::Y:
      glClearColor(val, val, 0.0, val);
      break;
    case glfw::KeyCode::K:
      glClearColor(0.0, 0.0, 0.0, val);
      break;
    default:
      break;
    }
  });

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);

    window.swapBuffers();
    glfw::pollEvents();
  }

  return 0;
}
