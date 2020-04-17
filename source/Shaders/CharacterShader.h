#pragma once

#include "ShaderProgram.h"

class CharacterShader : public ShaderProgram
{

public:
    CharacterShader(const char *vertexSource, const char *fragmentSource) : ShaderProgram(vertexSource, fragmentSource)
    {
        this->bind_attributes();
        this->get_uniforms();
    }

    ~CharacterShader()
    {
    }

    GLint ortho_matrix;
    GLint view_matrix;
    GLint character_matrix;

    // void start();
    // void stop();

protected:
    void bind_attributes() override;
    void get_uniforms() override;

    // void bind_attribute(int index, const GLchar *name);
    // GLint get_uniform_location(const GLchar *name);
};