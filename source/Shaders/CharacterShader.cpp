#include "CharacterShader.h"

void CharacterShader::bind_attributes()
{
    // this->bind_attribute(0, "position");
}

void CharacterShader::get_uniforms()
{
    this->ortho_matrix = this->get_uniform_location("orthoMatrix");
    this->character_matrix = this->get_uniform_location("characterMatrix");
    this->view_matrix = this->get_uniform_location("viewMatrix");
}