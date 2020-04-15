#pragma once

#include "..\Common.h"

#include "..\RawModel.h"
#include <vector>

class Loader
{
public:
    ~Loader()
    {
        this->clean();
    }

private:
    std::vector<GLuint> m_vaos;
    std::vector<GLuint> m_vbos;
    std::vector<RawModel*> m_models;

    GLuint create_vao();
    GLuint create_vbo();
    RawModel* create_model(GLuint id, int vertex_count);

    void buffer_data(GLfloat* vertices, int vertex_count);

public: 
    RawModel* load(GLfloat* vertices, int vertex_count);

    void clean();
};