# VLK

Utility and tool library for experiments with VULKAN SDK.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* Linux (>= 4.14): Actually we support Linux operating systems only. Testing is done on Ubuntu 18.
* g++ (>=7.4): We use C++ 2017. Testing is done with GCC 7.4 only.
* CMake (>= 3.12): The project's build system is based on CMake from Kitware (see licenses/cmake).CMake can be 
     downloaded from http://cmake.org or installed via ``` sudo apt install cmake```.
* Boost (>=1.62): We currently use boost for logging. Further uses of boost libraries are not excluded. See boost 
     license under licenses/boost. It can be obtained from http://boost.org or installed via 
     ```sudo apt install libboost1.62-all-dev```.
* glfw 3: Obtain this library from https://glfw.org, for licensing see licenses/glwf. Installation can be done via 
    ```sudo apt install libglfw3 libglfw3-dev```.
* glm: Provided on glm.g-truc.net or install as ```sudo apt install libglm-dev```. License is found under licenses/glm.
* gtest: For unit tests we actually use Google Test found on https://github.com/google/googletest. Installation should 
    done from source code directly because the Ubuntu packages (```libgtest-dev``) copy the required libraries to 
    location where CMake by default doesn't find them.
    
### Installing

1. Clone the repository code (or download a source archive and expand it) and create an out-of-source build directory:
   ```
    cd <your-development-directory>
    git clone https://github.com/alex-seifarth/vlk.git ./vlk
    mkdir vlk-build
    cd vlk-build
   ```
   
2. Execute initial cmake configuration:
   ```
     cmake ../vlk 
   ```
   or if you want to install (not ready yet) the library to a non-standard location:
   ```
    cmake -DCMAKE_INSTALL_PREFIX=<install-prefix-path> ../vlk
   ```
   
3. Build the library 
   ```
      make 
   ```
   
4. Installation !!! NOT READY YET !!!:
   ```
      make install 
   ```

## Running the tests
Unit test cases are actually built automatically with the library. To execute them enter the build directory 
(e.g. vlk-build if you followed the naming above) and run one of the following commands
   ```
   make test
   ctest
   ctest -V
   ```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the 
[tags on this repository](https://github.com/alex-seifarth/vlk/tags). 

## Authors
* **Alexander Seifarth** - *Initial Setup* 

See also the list of [contributors](https://github.com/alex-seifarth/contributors) who participated in this project.

## License

This project is licensed under the GPLv3 - see the [LICENSE](LICENSE) file for details

## Acknowledgments


