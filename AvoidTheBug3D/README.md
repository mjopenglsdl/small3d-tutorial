Avoid the Bug 3D
=================

![Demo 1](https://cloud.githubusercontent.com/assets/875167/18656425/4781b3d0-7ef1-11e6-83de-e412d5840fec.gif)

Move the goat around using the arrow keys, making sure the flying bug does not touch it, for the maximum amount of time.

*Runs on the [small3d](https://github.com/dimi309/small3d) game engine*

This branch is configured for a build with GLFW, using cmake (without the conan package manager). In order to build the game, first [build the small3d game engine](https://github.com/dimi309/small3d) with GLFW. Then, create a *build* directory inside the game's repository and copy the *build/include* and *build/lib* directories from the small3d build there. If you are using Windows, also copy the *build/bin* directory. Then, from the game's build directory, execute:
	
	cmake ..
	cmake --build .

And then just execute the **avoidthebug3d** executable from the **build/bin** directory. When using GLFW, you have to be inside the *build/bin* directory in order to do so, otherwise the program will not find the necessary resource files (shaders, textures, etc.) The reason is that GLFW does not offer a "get base directory" method so paths are assumed to begin from wherever the command to start the game is called. For your own projects, you can use code or a library to detect this directory, and pass it to small3d, but this is a demo game, so I did not want to expand the code too much or add more dependencies to the project.


