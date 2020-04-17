# EMGL BOILERPLATE /w DESKTOP COMPILATION #

Fully working C++ -> WebGL (Web assembly) emscripten boilerplate 
- DESKTOP SDL2 COMPILATION!
- Working Freetype
- ES 3.0 based
- Working antialiasing
- Mouse input (cross-language function calls)
- using SDL2
- detailed Windows setup

![alt text](https://i.imgur.com/vJoTmlm.png "Screenshot 1")

## Building the project... ##
`.wasm` and `.js` file is taken **from bin folder!**

- using make... `make` or `make web` from emsdk shell


## Setup ##

Setup done on windows (will work on other OSes, but you have to find tools from your own sources)

### MinGW (Windows) ###

1. [Download MinGW32](https://sourceforge.net/projects/mingw/)
2. Edit enviroment variables
    - add `C:\MinGW\bin` and `C:\MinGW\msys\1.0\bin`

### Emscripten ###

*done on Windows, will work on other oses but use ./ and .sh scripts*

1. Download latest version of [Python](https://www.python.org/) **DON'T FORGET TO ADD ENVIROMENT PATHS FOR PIP!**
2. Download latest version of [Clang LVVM](https://releases.llvm.org/download.html)
3. Download [Emscripten](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions)
    - open shell with **admin rights**
    - Clone newest repo `git clone https://github.com/emscripten-core/emsdk.git` and enter it's directory `cd emsdk`
    - `emsdk install latest` install latest sdk
    - `emsdk activate latest` (or later `emsdk_env.bat`) activate latest sdk 

### VSCode (emscripten intellisense support) ###

1. Install [C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
2. Edit `c_cpp_properties.json`
3. Include all needed paths... (see `.vscode/c_cpp_properties.json` for further reference)

    - c++ include path for `stdio` and stuff *(version may change)* `"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++"`
    - path for c++ `bits/..` *(needed by dependecies, version may change)* `"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\mingw32"`
    - path for desktop SDL2 `"C:\\libsdl\\include\\SDL2"` (https://www.libsdl.org/download-2.0.php)
    - path for GLEW wrangler *(gl calls)* `"C:\\libglew-2.1.0\\include"` (http://glew.sourceforge.net/)
    - path for FreeType library `"C:\\libfreetype-2.9\\include"` (https://www.freetype.org/download.html)
    
    Additional libraries like `glm` **(needed by this example)**, you may also use other libs this way in future
    - my glm include path `C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\glm`


4. **(OPTIONAL)** include paths for emscripten (see `.vscode/em_c_cpp_properties.json` for further reference)
    - Use this, if you need to access specific things in emscripten...
    - **EMSCRIPTEN PORTS LIKE SDL2 AND FREETYPE ARE GENERATED AFTER FIRST USE OF COMPILATION FLAG**

    - path for emscripten specific things `C:\\Program Files\\emsdk\\upstream\\emscripten\\system\\include"`
    - path for SDL2 *(includes gles with gl calls)* `"C:\\Users\\majda\\.emscripten_cache\\wasm\\include\\SDL2"`
    - path for FreeType "C:\\Users\\majda\\.emscripten_cache\\wasm\\ports-builds\\freetype\\include"

## Install needed libraries ##

- needed **SDL2, GLEW** and **FreeType**

- Great MinGW windows guide
    http://www.multigesture.net/articles/how-to-upgrade-your-mingw-with-commonly-used-libraries/

- You may use WSL to compile as well

## Code overview (antialiasing and other stuff) ## 

**Not done yet!**

1. Includes for **EMSCRIPTEN** - **DON'T USE THIS, USE DESKTOP SDL**
    - `#include <stdio.h>` and `#include <stdlib.h>` for `printf()` calls and other
    - `#include <emscripten.h>` for function binding and `#include <emscripten/html5.h>` for WebGL canvas context (enables antialiasing and stuf...)
    - `#include <SDL.h>` for all SDL2 calls
    - `#include <GLES3/gl3.h>` **(this .h file may be created after first use of USE_WEBGL2=1 compilation flag)** include that handles OpenGL ES 3.0 bindings
    - additional `#include <glm/ANYTHING>` for GLM library

2. window (canvas) created with `SDL_CreateWindow()`, attributes set with `SDL_GL_SetAttribute()`, swap buffers with `SDL_GL_SwapWindow(window)`
3. **Antialiasing!** *this part is done after SDL window creation and before GL manipulation*
    - ... setup SDL window 
    - create `EmscriptenWebGLContextAttributes attrs`, set `attrs.antialias = true` to **true**, you may also set minor/major versions and alpha
    - get (create) WebGL context `EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_context = emscripten_webgl_create_context("#canvas", &attrs)` *(`#canvas` identifies DOM canvas element)*
    - activate context `emscripten_webgl_make_context_current(webgl_context)`
    - ... use GL calls

4. Manual compilation
    - use `emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -o index.js -I"<LIBS LIKE GLM PATH>"` command from **emsdk shell* to compile code manually without any makefile

### Usefull links ###
- MinGW libaries: http://www.multigesture.net/articles/how-to-upgrade-your-mingw-with-commonly-used-libraries/
- Emscripten ports: https://github.com/emscripten-ports

### Inspired by... ###
- https://github.com/majda107/emgl-boilerplate

- https://github.com/krogank9/sdl_ogl_tests/blob/master/vg/src/main.cpp
- https://gist.github.com/derofim/261630cc9eea7fce431710dc2e97b094
- https://github.com/timhutton/opengl-canvas-wasm