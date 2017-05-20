#include <small3d/Renderer.hpp>
#include <small3d/SceneObject.hpp>
#include <GLFW/glfw3.h>

using namespace small3d;

bool down, left, right, up, esc, enter;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    down = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    up = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    left = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    right = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    esc = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    down = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    up = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    left = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    right = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    esc = false;

}
	
int main(int argc, char **argv) {
	
  Renderer *renderer = &Renderer::getInstance("Ball demo");

  GLFWwindow* window = renderer->getWindow();

  SceneObject ball("ball", "resources/ball.obj");
  ball.colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
  ball.offset = glm::vec3(0.0f, -1.0f, -8.0f);

  glfwSetKeyCallback(window, keyCallback);
	
  while (!glfwWindowShouldClose(window) && !esc) {

    glfwPollEvents();
    if (esc)
      break;
	
    if (up)
      ball.offset.z -= 0.1f;
    else if (down)
      ball.offset.z += 0.1f;
    else if (left)
      ball.offset.x -= 0.1f;
    else if (right)
      ball.offset.x += 0.1f;
	  
    renderer->clearScreen();
    renderer->render(ball);
    renderer->swapBuffers();
		
  }
	
  return 0;

}
