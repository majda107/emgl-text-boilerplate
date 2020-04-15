#include <stdio.h>

#include <stdlib.h>
#include <vector>
#include <string>

// #define __EMS__

#ifdef __EMS__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>
#include <GLES3/gl3.h>
#else

#define SDL_MAIN_HANDLED
#define main SDL_main

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::ortho
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

#include <ft2build.h>
#include FT_FREETYPE_H