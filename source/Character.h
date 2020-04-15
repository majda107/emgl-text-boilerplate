#pragma once

#include "Common.h"

#include "RawModel.h"

struct Character
{
    RawModel* model;
    GLuint texture_id;

    glm::ivec2 size;
    glm::ivec2 offset;
    GLuint advance;
};