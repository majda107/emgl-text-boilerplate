#include <stdio.h>

#include <stdlib.h>

// EMSCRIPTEN INCLUDES, you may IFDEFINE this for Desktop setup as well
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>
#include <GLES3/gl3.h>


#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::ortho
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr




// Shader sources

// Vertex shader
const GLchar *vertexSource =
    "attribute vec4 position;                     \n"
    "uniform mat4 orthoMatrix;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_Position = orthoMatrix * vec4(position.xyz, 1.0);     \n"
    "}                                            \n";

// Fragment/pixel shader
const GLchar *fragmentSource =
    // "precision mediump float;\n"
    "void main()                                  \n"
    "{                                            \n"
    "   gl_FragColor = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, .5, 1.0);\n"
    "}                                            \n";



// ugly GLOBALLY-SCOPED variables...
bool background = true;
SDL_Window *window;

GLint ortho_location;
glm::mat4x4 ortho = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);

// GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f}; // UNIFORM TRIANGLE
GLfloat vertices[] = {0.0f, 0.0f, 100.0f, 100.0f, 300.0f, 0.0f};




// magic that draws triangle
void invalidate_triangle()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    if (background)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(ortho_location, 1, 0, glm::value_ptr(ortho));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);
}



// external functions called by EMSCRIPTEN from javascript (index.html <script> tag) file using CWrap 
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color()
{
    background = !background;
    invalidate_triangle();
}

extern "C" void EMSCRIPTEN_KEEPALIVE draw_triangle(float x, float y)
{
    vertices[2] = x;
    vertices[3] = y;

    invalidate_triangle();
}



int main()
{

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);//only SDL_WINDOW_OPENGL may suffice
	if (!window) //if it doesn't work, lower the bar
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);//same as above
		if (!window) //if this doesn't work, it's probably becouse of an error. so we log it
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error2: sdl2 init failed: %s", SDL_GetError());
			return 1;
		}
	}



    // EMSCRIPTEN antialiasing magic
    EmscriptenWebGLContextAttributes attrs;
    attrs.antialias = true;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = false;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_context = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(webgl_context);




    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    ortho_location = glGetUniformLocation(shaderProgram, "orthoMatrix");

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);


    // set GL viewport
    glViewport(0, 0, 640, 480);


    return EXIT_SUCCESS;
}
