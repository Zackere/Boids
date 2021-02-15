#include "Main/Main.hpp"

#include <memory>
#include <set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Boids {
namespace {
bool main_running = false;
constexpr glm::vec3 kUp{0.f, 1.f, 0.f};

struct {
  float width = 2 * 1280.f, height = 2 * 960.f;
  float aspect = width / height;
} window_properties;
void OnResize(GLFWwindow*, int width, int height) {
  glViewport(0, 0, window_properties.width = width,
             window_properties.height = height);
  window_properties.aspect = window_properties.width / window_properties.height;
}

struct WindowDestroyer {
  void operator()(GLFWwindow* w) {
    if (w)
      glfwDestroyWindow(w);
  }
};

bool isKeyPressed(GLFWwindow* w, int key) {
  return glfwGetKey(w, key) == GLFW_PRESS;
}
}  // namespace
Main::Main() {
  glfwInit();
}

Main::~Main() {
  glfwTerminate();
}

int Main::Run() {
  if (main_running)
    return -1;
  main_running = true;
  std::unique_ptr<GLFWwindow, WindowDestroyer> window(
      glfwCreateWindow(window_properties.width, window_properties.height,
                       "Boids", nullptr, nullptr));
  if (!window)
    return -1;
  glfwMakeContextCurrent(window.get());
  if (glewInit() != GLEW_OK)
    return -1;
  glfwSetWindowSizeCallback(window.get(), OnResize);
  glViewport(0, 0, window_properties.width, window_properties.height);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);
  glClearColor(44.0f / 255.0f, 219.0f / 255.0f, 216.0f / 255.0f, 0.0f);

  do {
    glfwPollEvents();
    glfwSwapBuffers(window.get());
  } while (!isKeyPressed(window.get(), GLFW_KEY_ESCAPE) &&
           glfwWindowShouldClose(window.get()) == 0);
  main_running = false;
  return 0;
}
}  // namespace Boids
