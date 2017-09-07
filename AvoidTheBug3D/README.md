Avoid the Bug 3D
=================

![Demo 1](https://cloud.githubusercontent.com/assets/875167/18656425/4781b3d0-7ef1-11e6-83de-e412d5840fec.gif)

Move the goat around using the arrow keys, making sure the flying bug does not touch it, for the maximum amount of time.

*Runs on the [small3d](https://github.com/dimi309/small3d) game engine*

Add my bintray repository as a remote to your conan configuration. You only need to execute this once. Conan will remember it afterwards:

	conan remote add bintraydimi309 https://api.bintray.com/conan/dimi309/conan-packages
	
Also add jgsogo's bintray repository (that's where I get the portaudio dependency from):

	conan remote add bintrayjgsogo https://api.bintray.com/conan/jgsogo/conan-packages

On MacOS or Linux, build as follows, using the [conan](https://www.conan.io) package manager:

	git clone https://github.com/dimi309/small3d-tutorial
	cd small3d-tutorial/AvoidTheBug3D
	mkdir build
	cd build
	conan install .. --build missing
	cmake ..
	cmake --build .

On Microsoft Windows, better do it like this:

	git clone https://github.com/dimi309/small3d-tutorial
	cd small3d-tutorial/AvoidTheBug3D
	mkdir build
	cd build
	conan install .. --build missing
	cmake -G "Visual Studio 14 2015 Win64" ..
	cmake --build . --config Release

*(You may need a different cmake -G command, depending on your version of Visual Studio.)*

And then just execute the **avoidthebug3d** executable from the **build/bin** directory. You have to be inside the *build/bin* directory in order to do so, otherwise the program will not find the necessary resource files (shaders, textures, etc).
