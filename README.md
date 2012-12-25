About
---------------------
Project-Inglenook is a collection of processes designed to work together with the key goal to automate and integrate a multitude devices within a facility such as a home, office, hotel etc...

Features
---------------------
Project-Inglenook will provide:

* Module host process, to setup and communicate with each device's own module process
* Trigger management process, allowing devices events to trigger each other to create the ultimate network
* Multiple front-ends with web and mobile app interfaces (And with the vision to provide a 'Wall Embedded Touch Interfaces' too!), all exposed using a defined SOA so you can build-your-own!
* Security baked in, with multiple authentication methods and authorisation policies
* Full process logging, with tools to quick find what you are looking for
* System monitoring with automatic alerts if things are going wrong
* Software update process which can automatically patch the system depending on your preference

Development
---------------------
Please visit the wiki pages to see our current development progress... (Currently being migrated)

How to build
---------------------
First, checkout the git repository:

    git clone git://github.com/Inglenookians/Project-Inglenook.git
    cd Project-Inglenook

By default, you can just build the project using your default C++ compiler:

    ./setup-builds.sh
    cd build/debug (change to build/release for a release build)
    make 

To specify a compiler, use the CC and CXX flags (eg: for clang/clang++)

    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    ./setup-builds.sh
    cd build/debug (change to build/release for a release build)
    make 

To run the unit/integration tests:

    make test 

To perform an install:

    sudo make install 

To generate a RPM software package:

    cpack -G RPM CPackConfig.cmake

