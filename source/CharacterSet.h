#pragma once

#include "Common.h"

#include <unordered_map>

#include "Loaders/Loader.h"
#include "Character.h"

class CharacterSet
{
public:
    CharacterSet(const char *font, int height, Loader *loader)
    {
        printf("Character set constructor!\n");
        this->m_loader = loader;

        if (FT_Init_FreeType(&(this->m_ft)))
            printf("FAILED TO INIT FREETYPE!\n");
    }

    ~CharacterSet()
    {
        this->clean();
        FT_Done_FreeType(this->m_ft);
    }

private:
    FT_Library m_ft;
    const char *m_font;

    Loader *m_loader;

    GLuint generate_texture(FT_Face face);
    void generate_model(Character *character);

public:
    std::unordered_map<char, Character *> character_map;

    void generate_character(char letter);

    void done()
    {
        FT_Done_FreeType(this->m_ft);
    }

    void clean();
};