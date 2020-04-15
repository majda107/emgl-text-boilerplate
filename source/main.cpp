#include "Common.h"

#include "Character.h"
#include "CharacterSet.h"
#include "Loaders/Loader.h"

#include <string>

// Shader sources

// Vertex shader
const GLchar *vertexSource =
    "#version 300 es\n"
    "layout (location = 0) in vec4 position;                     \n"
    "uniform mat4 orthoMatrix;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_Position = orthoMatrix * vec4(position.xyz, 1.0);     \n"
    "}                                            \n";

// Fragment/pixel shader
const GLchar *fragmentSource =
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 color;"
    "void main()                                  \n"
    "{                                            \n"
    "   color = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, .5, 1.0);\n"
    "}                                            \n";

const GLchar *glyphVertexSource =
    "#version 300 es \n"
    "layout (location = 0) in vec4 position;                     \n"
    "uniform mat4 orthoMatrix;\n"
    "uniform mat4 characterMatrix;\n"
    "out vec2 textureCoords; \n"
    "void main()                                  \n"
    "{                                            \n"
    "  vec4 worldPosition = characterMatrix * vec4(position.xy, .0, 1.0); \n"
    "  gl_Position = orthoMatrix * worldPosition;     \n"
    "  textureCoords = position.zw; \n"
    "}                                            \n";

// Fragment/pixel shader
const GLchar *glyphFragmentSource =
    "#version 300 es \n"
    "precision mediump float;\n"
    "in vec2 textureCoords; \n"
    "uniform sampler2D text; \n"
    "out vec4 color;\n"
    "void main()                                  \n"
    "{                                            \n"
    // "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoords).a);"
    // "   gl_FragColor = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, .5, 1.0) * sampled;\n"
    "   color = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, .5, 1.0);\n"
    // "   color = vec4(texture(text, textureCoords).a, 0.1, 0.1, 1.0); \n"
    "   color.rgb = mix(vec3(.0, .0, .0).xyz, color.rgb, texture(text, textureCoords).a); \n"
    "}                                            \n";

// ugly GLOBALLY-SCOPED variables...
bool background = true;
SDL_Window *window;

GLint ortho_location;
GLint otrho_glyph_location;
glm::mat4x4 ortho = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);

GLint character_matrix_location;
glm::mat4x4 character_matrix = glm::mat4x4(1.0);

GLint triangle_program;
GLint glyph_program;

// GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f}; // UNIFORM TRIANGLE
GLfloat vertices[] = {0.0f, 0.0f, 100.0f, 100.0f, 300.0f, 0.0f};

GLuint triangleVAO;
GLuint triangleVBO;

CharacterSet *set;
Loader *loader;

std::string test_str = "test string";

// magic that draws triangle
void invalidate_triangle()
{
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glUseProgram(triangle_program);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 2 * 3, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (background)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(ortho_location, 1, 0, glm::value_ptr(ortho));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glUseProgram(0);

    SDL_GL_SwapWindow(window);
}

void invalidate_string(std::string string)
{
    character_matrix = glm::mat4x4(1.0);
    character_matrix *= glm::translate(character_matrix, glm::vec3(10, 200, 0));

    glUseProgram(glyph_program);

    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(otrho_glyph_location, 1, 0, glm::value_ptr(ortho));

    // int offset = 0;
    for (auto it = string.begin(); it != string.end(); it++)
    {
        auto glyph = set->character_map[*it];

        // printf("Drawing %c\n", *it);

        glUniformMatrix4fv(character_matrix_location, 1, 0, glm::value_ptr(character_matrix));

        glBindVertexArray(glyph->model->id);
        glBindTexture(GL_TEXTURE_2D, glyph->texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        character_matrix *= glm::translate(glm::mat4x4(1.0), glm::vec3((glyph->advance >> 6 ), 0, 0));
        // offset += glyph->advance;
    }

    glBindVertexArray(0);

    glUseProgram(0);

    SDL_GL_SwapWindow(window);
}

void invalidate()
{
    glClear(GL_COLOR_BUFFER_BIT);
    invalidate_string(test_str);
}

#ifdef __EMS__
// external functions called by EMSCRIPTEN from javascript (index.html <script> tag) file using CWrap
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color()
{
    background = !background;
    invalidate();
}

extern "C" void EMSCRIPTEN_KEEPALIVE draw_triangle(float x, float y)
{
    vertices[2] = x;
    vertices[3] = y;

    invalidate();
}

extern "C" void EMSCRIPTEN_KEEPALIVE append_letter(char letter)
{
    test_str.push_back(letter);
}

extern "C" void EMSCRIPTEN_KEEPALIVE remove_letter()
{
    if(test_str.size() <= 0) return;
    test_str.pop_back();
}

#endif

void check_print_shader(GLint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("Shader compilation status: %d\n", status);
    if (status == GL_FALSE)
    {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        std::vector<GLchar> log(logLen);
        glGetShaderInfoLog(shader, logLen, &logLen, &log[0]);

        for (auto it = log.begin(); it != log.end(); it++)
            printf("%c", *it);

        printf("\n");
    }
}

GLuint create_shader_program(const GLchar *vertex_source, const GLchar *fragment_source)
{
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_source, nullptr);
    glCompileShader(vertexShader);
    check_print_shader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_source, nullptr);
    glCompileShader(fragmentShader);
    check_print_shader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}

void clean()
{
    delete set;
    delete loader;
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); //only SDL_WINDOW_OPENGL may suffice
    if (!window)                                                                                                       //if it doesn't work, lower the bar
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
        window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); //same as above
        if (!window)                                                                                                       //if this doesn't work, it's probably becouse of an error. so we log it
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error2: sdl2 init failed: %s", SDL_GetError());
            return 1;
        }
    }

#ifdef __EMS__
    // EMSCRIPTEN antialiasing magic
    EmscriptenWebGLContextAttributes attrs;
    attrs.antialias = true;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = false;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_context = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(webgl_context);

#else

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    glDepthMask(false);

    glewInit();

    char *version = (char *)glGetString(GL_VERSION);
    printf("Runinng version: %s", version);

#endif

    // Create a Vertex Buffer Object and copy the vertex data to it

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    triangle_program = create_shader_program(vertexSource, fragmentSource);
    ortho_location = glGetUniformLocation(triangle_program, "orthoMatrix");

    glyph_program = create_shader_program(glyphVertexSource, glyphFragmentSource);
    otrho_glyph_location = glGetUniformLocation(glyph_program, "orthoMatrix");
    character_matrix_location = glGetUniformLocation(glyph_program, "characterMatrix");

    // set GL viewport
    glViewport(0, 0, 640, 480);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    loader = new Loader();
    set = new CharacterSet("fonts/arial.ttf", 128, loader);

    for(int i = 0; i < 128; i++)
        set->generate_character((char)i);



    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);
    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

// LOOP FOR DESKTOP VERSION
#ifndef __EMS__
    while (true)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit(0);
                break;
            }
        }

        invalidate();
    }

    clean();
#endif

    return EXIT_SUCCESS;
}