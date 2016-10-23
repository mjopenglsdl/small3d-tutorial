#include <small3d/Renderer.hpp>
#include <small3d/SceneObject.hpp>

using namespace small3d;
	
int main(int argc, char **argv) {
	
	Renderer renderer("Ball demo");
	
	SceneObject ball("ball", "resources/ball.obj");
	ball.colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	ball.offset = glm::vec3(0.0f, -1.0f, -8.0f);
	
	while(true){
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				break;
			}
	  }
	  
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	
	if (keyState[SDL_SCANCODE_UP] == 1)
		ball.offset.z -= 0.1f;
	else if (keyState[SDL_SCANCODE_DOWN] == 1)
		ball.offset.z += 0.1f;
	else if (keyState[SDL_SCANCODE_LEFT] == 1)
		ball.offset.x -= 0.1f;
	else if (keyState[SDL_SCANCODE_RIGHT] == 1)
		ball.offset.x += 0.1f;
	  
	renderer.clearScreen();
	renderer.render(ball);
	renderer.swapBuffers();
		
	}
	
return 0;

}
