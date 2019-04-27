# tsukimade
A set of C++14 headers which makes it easier to write Lua bindings of C functions.
At this stage, it only supports Lua 5.2 while other versions will be considered in future.

This project is released under MIT License, see LICENSE.


## Install
Copy header files in "include" directory to your project folder.


## Documentation
The project uses Doxygen (http://www.doxygen.nl/) to generate a reference manual.
To build documents, type

    cmake .

or something like this on your shell.

The command above also builds a test program provided the following packages are found:
- pkg-config
- liblua5.2-dev
Note that the source codes for tests, which are written in C, C++, and Lua, include some examples of the usage of the library.

