DISCONTINUATION OF PROJECT.

This project will no longer be maintained by Intel.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project. 

Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project. 
# Collision Avoidance Library #

[![Build Status](https://travis-ci.org/01org/collision-avoidance-library.svg?branch=master)](https://travis-ci.org/01org/collision-avoidance-library) <a href="https://scan.coverity.com/projects/01org-collision-avoidance-library">
  <img alt="Coverity Scan Build Status"
         src="https://scan.coverity.com/projects/11862/badge.svg"/>
</a>

A framework for testing and benchmarking collision avoidance strategies.

## Requirements ##

 * CMake 3.1 or newer
 * Gazebo 8.0+ (for virtual camera/vehicle support)
 * socat 1.7+ (for testbed support)
 * GZSitl (for virtual vehicle support)
 * GLM (https://github.com/g-truc/glm.git)
 * Autopilot, either:
   * Ardupilot (https://github.com/ArduPilot/ardupilot) or
   * PX4 (https://github.com/PX4/Firmware)

Collision Avoidance Library (Coav) is developed having drones in mind, so when compiling
the library without additional options, features related to benchmark and simulation
will be OFF by default. This should be the preferred way when you want to ship
the library on your drone target/product.

### Library Features and Options ###

Collision Avoidance Library has support to following features that can be defined
on compile time:

Feature/Option          | Compile Options | Default Value
----------------------- | --------------- | -------------
Intel RealSense support | WITH_REALSENSE  | ON
Gazebo support          | WITH_GAZEBO     | OFF
Visual Debugger support | WITH_VDEBUG     | OFF (depends on Gazebo)
Coav Tools              | WITH_TOOLS      | OFF
Compile code samples    | WITH_SAMPLES    | OFF

## Build and Install ##

### Method 1 - Embed 'coav-control' on an Intel Aero image ###

This method is recommended for those who want to use 'coav-control' on an Intel
Aero Drone.

A Yocto layer containing recipes to build and install coav-control can be found on
the repository under the folder 'meta-coav'. This layer can be easily added to
to the image build by following the steps described by Intel Aero documentation
[here](https://github.com/intel-aero/meta-intel-aero/wiki/Developing-on-Intel-Aero).

The recipe install the 'coav-control' utility tool as well an init script that runs
the tool on start-up. You can change it's behavior by editing the script file at
any point of the process that seems convenient to you (custom branch, custom recipe
or changing the file on the drone itself).

### Method 2 - Compile and Install yourself ###

This method is recommended for those who will run simulations and tests on the
development environment instead of a real drone. It is also recommend for those
actively writing the library code because makes it easier to switch binaries
for tests during development. If targeting an Intel Aero drone, check [additional
instructions](#deploying-on-intel-aero) about taking advantage of Yocto's SDK support.

If you're using Ubuntu, before continuing please ensure you have the needed dependencies:
 * If you want to use Gazebo, ensure you go through the instructions available [here](http://gazebosim.org/tutorials?tut=install_ubuntu) and ensure you install the libgazebo8-dev package;
 * Install all build dependencies (the last two are needed to build librealsense):
 
  ```
  sudo apt-get install git cmake libglm-dev python-future doxygen libusb-1.0-0-dev libglfw3-dev
  ```

 * Go through the steps to install librealsense (it does not support librealsense2) which can be found [here](https://github.com/IntelRealSense/librealsense/blob/legacy/doc/installation.md)
  
The project use CMake as build system and does not support in-tree build.
As such, create a separate folder before building.

1. Make sure you have initialized and updated all the required submodules at
least once with:

    ```
    git submodule update --init --recursive
    ```

2. Create a "build" folder and build the library using CMake as follows:

    ```
    mkdir build
    cd build
    cmake ..
    sudo make install
    cd -
    ```
    These instructions will build and install the targets on cmake's
    default install path (usually '/usr/local'). To modify the library options,
    the following syntax is used when issuing `cmake`:

    ```
    cmake .. -D<COMPILE_OPTION_1>=<OPTION_1_VALUE> -D<COMPILE_OPTION_2>=<OPTION_2_VALUE>
    ```

    Also, the following CMake options may be of value:

    Option | Description
    --- | ---
    CMAKE_INSTALL_PREFIX | Set a custom installation path. This path is also used for dependency search.
    CMAKE_PREFIX_PATH | Add paths to be searched when looking for dependencies

    A more complete explanation of those options can be found on CMake's Documentation.

    Example:

      * Search GLM and Mavlink on <custom_deps_path>
      * Change the install path to <custom_install_path>
      * Compile the library additional tools (coav-sim)

    ```
    cmake .. -DCMAKE_INSTALL_PREFIX=<custom_install_path> -DCMAKE_PREFIX_PATH=<custom_deps_path> -DWITH_TOOLS=ON
    ```

## Testing Collision Avoidance Library with *coav-control* ##

Make sure that the library was compiled with 'Coav Tools' turned on. This will
build a target `coav-control` that can be found in 'tools/coav-control/' inside
the build folder.

`coav-control` can be used execute a simple collision avoidance system for a
Mavlink controlled Quadcopter that is composed by: a sensor, a detection algorithm
and a detection strategy. It needs interaction with an autopilot and a sensor to work,
so it won't do much when executed alone.

The following will list the possible options for each component:

```
./coav-control --help
```

Example:

Run a collision avoidance system composed by:
 * Intel Realsense
 * Obstacle detector based on 'Blob extraction'
 * 'Stop' avoidance strategy

```
./coav-control -d DI_OBSTACLE -a QC_STOP -s ST_REALSENSE
```

## Simulation and Automated tests ##

For information on how to make use of 'Collision Avoidance Library' on simulated
environment and how to take advantage of tests automation via testbed, please
refer to the [Simulation Docs](https://github.com/01org/collision-avoidance-library/wiki/Quickstart#running-the-simulation).

## Deploying on Intel Aero ##

Intel Aero firmware is based on Yocto, so the Yocto SDK for Intel Aero will be
used to properly compile Collision Avoidance Library for deploy on Intel Aero.

Instruction on how to build Intel Aero image and the associated SDK can be found
on Intel Aero [Wiki](https://github.com/intel-aero/meta-intel-aero/wiki).

Intel Aero SDK will be missing one of the Collision Avoidance Library
dependencies:
  * GLM

Since GLM is a "headers only" library, cmake just need to know where to find
the headers in order to successfully "cross-compile" it. This will be
done with "-DCMAKE_PREFIX_PATH" parameter as described by the instructions
bellow.

Once Intel Aero SDK is successfully installed, the following instructions will
configure the environment and compile the library:

    source <SDK_PATH>/environment-setup-core2-64-poky-linux

    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH="<GLM_HEADERS_PATH>"
    make

After a successful build, you can install Collision Avoidance Library in a
temporary path:

    make install DESTDIR=<TMP_PATH>

Pack everything:

    cd <TMP_PATH>
    tar cvf coav.tar *

Copy coav.tar to Intel Aero root dir and execute the following on Intel Aero:

    [intel-aero]$ cd /
    [intel-aero]$ tar xvf coav.tar

And Collision Avoidance Library should be successfully installed!
