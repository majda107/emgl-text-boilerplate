#pragma once

#include "..\Common.h"

class ShaderProgram
{
public:
    ShaderProgram(const GLchar *vertex, const GLchar *fragment)
    {
        this->m_vertex = create_shader(vertex, GL_VERTEX_SHADER);
        this->m_fragment = create_shader(fragment, GL_FRAGMENT_SHADER);
        this->m_program = glCreateProgram();

        glAttachShader(this->m_program, this->m_vertex);
        glAttachShader(this->m_program, this->m_fragment);
        glLinkProgram(this->m_program);
    }

    virtual ~ShaderProgram()
    {

    }

private:
    GLuint create_shader(const GLchar *source, GLenum type);
    void check_print_shader(GLuint shader);

    GLuint m_vertex;
    GLuint m_fragment;
    GLuint m_program;

protected:
    virtual void bind_attributes() = 0;
    virtual void get_uniforms() = 0;

    void bind_attribute(int index, const GLchar *name);
    GLint get_uniform_location(const GLchar *name);

public:
    void start();
    void stop();
};