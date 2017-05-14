small3d - Getting Started
=========================

The steps below will work on Windows, OSX/MacOS and Linux. The source code for this tutorial, including the model of a ball we will be creating, is available in this repository. If you encounter difficulties [let me know](https://github.com/dimi309/small3d-tutorial/issues).

We are going to create a ball that can be moved using the keyboard arrows. Even though small3d is small, it can do a lot more than this. This exercise will just get you started. You can then have a look at the source code of two games that have already been developed with the engine ([Avoid the Bug 3D](https://github.com/dimi309/small3d-tutorial/tree/master/AvoidTheBug3D) and [Chase the Goat 3D](https://github.com/dimi309/small3d-tutorial/tree/master/ChaseTheGoat3D)). The API documentation is also [available online](http://dimi309.github.io/small3d/).

I assume that you already have your compiler set up. You also need to install [cmake](https://cmake.org) and [conan](https://www.conan.io) and make sure they can be executed from the command line. I prefer to use the engine by deploying it from conan.io and that's what I'm doing in the tutorial below.

To begin with, let's make a directory for our ball-moving masterpiece from the command line:

	mkdir ball

Also, create a "resources" directory inside the "ball" directory:

	cd ball
	mkdir resources

Let's start from the code-less part. We need a Wavefront file containing a ball. You can use any tool that exports this format to create one, but I am using [Blender](https://www.blender.org), so I'll show you how to do this there. When you start Blender, you see a cube:

![blendercube](https://cloud.githubusercontent.com/assets/875167/19621157/15ee0f9e-988c-11e6-9c8a-b871bd7cdfa9.png)

Press "a" to select the cube. If the cube is selected already, pressing "a" will de-select it. Press it again in that case. Then "x" to delete the cube. You will be asked to confirm the deletion:

![blenderconfirmdeletion](https://cloud.githubusercontent.com/assets/875167/19621164/2c33f778-988c-11e6-8dc0-aec7be490bcc.png)

Just press enter to do so. Then, from the menu at the bottom left of the 3D view, select Add > Mesh > UV Sphere:

![blenderaddsphere](https://cloud.githubusercontent.com/assets/875167/19621171/41a9a346-988c-11e6-9522-02972fdfc57e.png)

This will create, as the name implies, a sphere:

![blendersphere](https://cloud.githubusercontent.com/assets/875167/19621175/561dfc78-988c-11e6-94f0-711369ce364b.png)

With the sphere selected (use the "a" key if it is not), click on the "Smooth" button, under "Shading" on the "Edit" menu on the left of the screen:

![smoothshading](https://cloud.githubusercontent.com/assets/875167/19621180/6ad2ff56-988c-11e6-96bf-36df739e85e7.png)

This is important for the way the sphere will be exported. We now need to create the Wavefront file. From the menu at the top, select File > Export > Wavefront (.obj). We need to set some options on the "Export OBJ" menu on the left. Only select "Write Normals", "Triangulate Faces" and "Keep Vertex Order":

![wavefrontexportoptions](https://cloud.githubusercontent.com/assets/875167/19621183/7da81a76-988c-11e6-9e50-604699e62900.png)

Save the exported file as "ball.obj" in the "resources" directory, created earlier (there is a location and name selector at the top and an "Export OBJ" button to actually save the file).

Now we are ready for the code. Create a file called "main.cpp" in the "ball" directory, with your editor or IDE. For starters let's just make our program use small3d. One class that we will definitely need is Renderer. This one creates a window and, as the name implies, takes care of all the rendering. So here is our main.cpp, with Renderer included:

	#include <small3d/Renderer.hpp>
	
	int main(int argc, char **argv) {
		
		return 0;
	}

Notice that we have not downloaded small3d from anywhere. Let's tell conan to do that for us. Create a file called "conanfile.txt" in the "ball" directory. Inside that file we declare small3d as our dependency:

	[requires]
	small3d/master@coding3d/stable

We also need to mention that we will be working with cmake:

	[generators]
	cmake

For Windows, we will need the dlls of the libraries we are using (small3d and its dependencies will be automatically downloaded by conan and some of them are not configured to be statically linked) to be copied to our binary output directory. Let's tell conan to take care of that for us:

	[imports]
	bin, *.dll -> ./bin

If you don't use Windows, you can still add that to your conanfile. It will allow your program to compile and run on Windows, should you ever want to do that and, on any other system, it will cause no problem since conan will not crash or anything if it doesn't find any dlls. It just won't copy them.

Finally, we are going to need the small3d shaders. Let's tell conan to also copy those, under [imports]:

	[imports]
	bin, *.dll -> ./bin
	shaders, * -> ./bin/resources/shaders

So the whole conanfile.txt will look like this:

	[requires]
	small3d/master@coding3d/stable
	
	[generators]
	cmake
	
	[imports]
	bin, *.dll -> ./bin
	shaders, * -> ./bin/resources/shaders
	

Let's see if it works. We are going to be building in a separate directory, in order to keep things clean. From inside the "ball" directory, execute:

	mkdir build
	cd build
	conan install .. --build missing

Conan will download small3d and all libraries that it depends on and place them in a local cache. It will also create its own cmake configuration file (conanbuildinfo.cmake) and, as instructed, copy small3d's shaders to the bin/resources/shaders directory. We are now ready to create our cmake configuration. Back inside the "ball" directory, let's create a CMakeLists.txt file. We will set the minimum required cmake version and declare our project:

	cmake_minimum_required(VERSION 3.0.2)
	project(demo)

Then we will link our project with conan, by including the conan cmake configuration created in the previous step and then setting up conan with a command contained in it:

	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()

We can now declare our executable, also linking it with the conan downloaded libraries:

	add_executable(ball main.cpp)

We link the libraries (managed by conan):

	target_link_libraries(ball PUBLIC "${CONAN_LIBS}")
	
Each library may require some special link flags. Conan knows about those too, so we'll let it take care of them for us:

	set_target_properties(ball PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

And, finally, in order for our compiled program to be able to easily access our ball model, which we created earlier, let's have it copied by cmake to the binary output directory:

	file(copy "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")

So the whole CMakeLists.txt file should look like this:

	cmake_minimum_required(VERSION 3.0.2)
	project(demo)

	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()

	add_executable(ball main.cpp)

	target_link_libraries(ball PUBLIC "${CONAN_LIBS}")
	set_target_properties(ball PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

	file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")

Let's see if everything works:

	cd build
	cmake ..
	cmake --build .

On Windows, you need to do this, adding some configuration parameters, depending on your development setup. For example:

	cd build
	cmake -G "Visual Studio 14 2015 Win64" ..
	cmake --build . --config Release

This will compile our program. Inside the build/bin directory, there should be a ball (or ball.exe) executable. At this point though, it doesn't do much. Time to add our ball to the mix. Back in main.cpp, we include small3d's SceneObject class, right under the inclusion of the renderer class (or above it, it doesn't matter):

	#include <small3d/Renderer.hpp>
	#include <small3d/SceneObject.hpp>

Now we need the GLFW header files:

	#include <GLFW/glfw3.h>

We also need to be using the small3d namespace, so this goes under our include statements:

	using namespace small3d;
	
We also need to write the logic that will be detecting key presses:

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

And finally, we go to the main program, and we create the renderer:

	Renderer renderer("Ball demo");

We create the ball:

	SceneObject ball("ball", "resources/ball.obj");

Let's say it's yellow:

	ball.colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

small3d uses vectors a lot as parameters for convenience. In this case, the vector symbolises an rgb colour, together with the alpha channel (last component).

On the other hand, when positioning the ball, the components are in order, x (-left, +right), y(+up, -down), and z(-away from the camera, +towards the camera):

	ball.offset = glm::vec3(0.0f, -1.0f, -8.0f);

So let's start our main loop now. small3d uses GLFW and you can use it too! First we need to declare the callback function, which will be the keyCallback method we wrote above.

	glfwSetKeyCallback(window, keyCallback);

Now in every iteration, we need to check whether we want to exit the program. Let's say that we'll be doing that with the Esc key:

	while (!glfwWindowShouldClose(window) && !esc) {

	glfwPollEvents();
		if (esc)
			break;

If after that we are still in the loop (so, no Esc key pressed), we will want to move the ball around with the keyboard. 

We will have the up arrow move the ball away from the camera. Down will do the opposite. Guess what left and right will do :)

	if (up)
      ball.offset.z -= 0.1f;
    else if (down)
      ball.offset.z += 0.1f;
    else if (left)
      ball.offset.x -= 0.1f;
    else if (right)
      ball.offset.x += 0.1f;

Ok, the ball is positioned. Now we need to actually draw it. We clear the screen first:

	renderer.clearScreen();

Then we render the ball:
	
	renderer.render(ball);

We are using a double-buffered system (we draw on one buffer, while the user is looking at the other one), so we also need to swap the buffers:

	renderer.swapBuffers();

And we close the loop :)

	}

That's it! Remember you can review the full listing of this program in this GitHub repository.

Let's try it out:

	cd build
	cmake --build .
	./bin/ball

For Windows:

	cd build
	cmake --build . --config Release
	.\bin\ball.exe

There's our ball:

![ball](https://cloud.githubusercontent.com/assets/875167/19624720/2fb6b99e-9904-11e6-885c-504ba726eeec.png)

Try moving it around with the arrows.
