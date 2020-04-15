#include "Loader.h"

GLuint Loader::create_vao()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);

    this->m_vaos.push_back(vao);
    return vao;
}

GLuint Loader::create_vbo()
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    this->m_vbos.push_back(vbo);
    return vbo;
}

RawModel* Loader::create_model(GLuint vao, int vertex_count)
{
    auto model = new RawModel(vao, vertex_count);

    this->m_models.push_back(model);
    return model;
}


void Loader::buffer_data(GLfloat* vertices, int vertex_count)
{
    auto buffer = this->create_vbo();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RawModel* Loader::load(GLfloat* vertices, int vertex_count)
{
    auto vao = this->create_vao();

    glBindVertexArray(vao);
    this->buffer_data(vertices, vertex_count);
    glBindVertexArray(0);

    return this->create_model(vao, vertex_count);
}

void Loader::clean() 
{
    glDeleteVertexArrays(this->m_vaos.size(), &m_vaos[0]);
    glDeleteVertexArrays(this->m_vbos.size(), &m_vbos[0]);

    for(auto it = this->m_models.begin(); it != this->m_models.end(); it++)
        delete *it;
}