Avoid the Bug 3D
=================

![Demo 1](https://cloud.githubusercontent.com/assets/875167/18656425/4781b3d0-7ef1-11e6-83de-e412d5840fec.gif)

Move the goat around using the arrow keys, making sure the flying bug does not touch it, for the maximum amount of time.

*Runs on the [small3d](https://github.com/dimi309/small3d) game engine*

In order to build the game, first [build the small3d game engine](https://github.com/dimi309/small3d) with GLFW. Then, create a *deps* directory inside the game's repository and copy the *cmake*, *build/include* and *build/lib* directories from the small3d build there. If you are using Windows, also copy the *build/bin* directory. Then, create a *build* directory and, from inside it, execute:
	
	cmake ..
	cmake --build .

And then just execute the **avoidthebug3d** executable from the **build/bin** directory. You have to be inside the *build/bin* directory in order to do so, otherwise the program will not find the necessary resource files (shaders, textures, etc).