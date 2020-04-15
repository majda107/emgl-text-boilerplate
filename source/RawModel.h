#pragma once

#include "Common.h"

struct RawModel
{
public:
    RawModel(GLuint id, int vertex_count)
    {
        this->id = id;
        this->vertex_count = vertex_count;
    }

public:
    GLuint id;
    int vertex_count;
};