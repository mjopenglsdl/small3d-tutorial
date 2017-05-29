Chase the Goat 3D
=================

![Demo 2](https://cloud.githubusercontent.com/assets/875167/18656844/0dc828a0-7ef5-11e6-884b-706369d682f6.gif)

Chase the goat. Accelerate using the spacebar and navigate using the arrow keys.

Before proceeding, please note that conan packages will be stored on bintray.com in the future. I have already made the switch so, just once, if you have not already done so, you need to declare my bintray repository as a remote, so that you can download the referenced packages:

    conan remote add bintraydimi309 https://api.bintray.com/conan/dimi309/conan-packages

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

*(You may need a different cmake -G command, depending on your version of Visual Studio.)*

And then just execute the **chasethegoat3d** executable from the **build/bin** directory. You have to be inside the *build/bin* directory in order to do so, otherwise the program will not find the necessary resource files (shaders, textures, etc).
