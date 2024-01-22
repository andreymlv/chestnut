#include <glad/gl.h>

#include <glfwpp/glfwpp.h>

#include <iostream>
#include <vector>

int main() {
  const char *vshader(R"(#version 460 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})");
  const char *fshader(R"(#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})");

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

  if (!gladLoadGL(glfwGetProcAddress)) {
    printf("Failed to initialize OpenGL context\n");
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

  int success;
  char infoLog[512];
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vshader, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fshader, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  std::vector<float> vertices = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    window.swapBuffers();
    glfw::pollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  return 0;
}
