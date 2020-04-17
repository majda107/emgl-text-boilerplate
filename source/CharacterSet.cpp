#include "CharacterSet.h"

GLuint CharacterSet::generate_texture(FT_Face face)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void CharacterSet::generate_model(Character *character)
{
    GLfloat xpos = character->offset.x;
    GLfloat ypos = -character->size.y + character->offset.y;
    // GLfloat ypos = -character->offset.y;

    GLfloat w = character->size.x;
    GLfloat h = character->size.y;

    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 0.0},
        {xpos, ypos, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 1.0},

        {xpos, ypos + h, 0.0, 0.0},
        {xpos + w, ypos, 1.0, 1.0},
        {xpos + w, ypos + h, 1.0, 0.0}};

    character->model = this->m_loader->load(vertices[0], 6 * 4);
}

void CharacterSet::generate_character(char letter)
{
    FT_Face face;
    if(FT_New_Face(this->m_ft, "fonts/arial.ttf", 0, &face))
        printf("FAILED TO INIT FACE IN FT!\n");

    // FT_Set_Pixel_Sizes(face, 0, 48);
    FT_Set_Pixel_Sizes(face, 0, 64);

    FT_Load_Char(face, letter, FT_LOAD_RENDER);

    printf("Face of X built.. : %d by %d\n", face->glyph->bitmap.width, face->glyph->bitmap.rows);

    auto character = new Character;
    character->texture_id = this->generate_texture(face);
    character->size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    character->offset = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    character->advance = glm::vec2(face->glyph->advance.x, face->glyph->advance.y);

    this->generate_model(character);

    this->character_map.insert({letter, character});

    FT_Done_Face(face);
}

void CharacterSet::clean()
{
    auto it = this->character_map.begin();
    while (it != this->character_map.end())
    {
        delete it->second;
        it++;
    }
}
