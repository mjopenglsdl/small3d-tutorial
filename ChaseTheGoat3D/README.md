Chase the Goat 3D
=================

![Demo 2](https://cloud.githubusercontent.com/assets/875167/18656844/0dc828a0-7ef5-11e6-884b-706369d682f6.gif)

Chase the goat. Accelerate using the spacebar and navigate using the arrow keys.

On MacOS or Linux, build as follows, using the [conan](https://www.conan.io) package manager:

	git clone https://github.com/compiletoplay/ChaseTheGoat3D
	cd ChaseTheGoat3D
	mkdir build
	cd build
	conan install .. --build missing
	cmake ..
	cmake --build .

On Microsoft Windows, better do it like this:

	git clone https://github.com/compiletoplay/ChaseTheGoat3D
	cd ChaseTheGoat3D
	mkdir build
	cd build
	conan install .. --build missing
	cmake -G "Visual Studio 14 2015 Win64" ..
	cmake --build . --config Release

And then just execute the **chasethegoat3d** executable from the **build/bin** directory.

*Runs on the [small3d](https://github.com/dimi309/small3d) game engine*
