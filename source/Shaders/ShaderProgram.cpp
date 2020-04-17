#include "ShaderProgram.h"

GLuint ShaderProgram::create_shader(const GLchar *source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    this->check_print_shader(shader);

    return shader;
}

void ShaderProgram::check_print_shader(GLuint shader)
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

void ShaderProgram::bind_attribute(int index, const GLchar *name)
{
    glBindAttribLocation(this->m_program, index, name);
}

GLint ShaderProgram::get_uniform_location(const GLchar *name)
{
    return glGetUniformLocation(this->m_program,name);
}

void ShaderProgram::start()
{
    glUseProgram(this->m_program);
}

void ShaderProgram::stop()
{
    glUseProgram(0);
}
