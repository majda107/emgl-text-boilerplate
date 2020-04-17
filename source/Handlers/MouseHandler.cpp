#include "MouseHandler.h"

void MouseHandler::mouse_down(glm::vec2 pos)
{
    this->m_down = true;
    this->m_pos = pos;
}

void MouseHandler::mouse_move(glm::vec2 pos)
{
    if (!(this->m_down))
        return;

    auto dev = this->m_pos - pos;
    this->set_drag(this->get_drag() + dev);

    this->m_pos = pos;
}

void MouseHandler::mouse_up(glm::vec2 pos)
{
    this->m_down = false;
    this->m_pos = pos;
}

bool MouseHandler::get_down()
{
    return this->m_down;
}

glm::vec2 MouseHandler::get_drag()
{
    return this->m_drag;
}

void MouseHandler::set_drag(glm::vec2 drag)
{
    this->m_drag = drag;
    this->generate_view_matrix();
}

glm::mat4x4 MouseHandler::get_view_matrix()
{
    return this->m_view_matrix;
}

float MouseHandler::get_zoom()
{
    return this->m_zoom;
}

void MouseHandler::set_zoom(float value)
{
    printf("SETTING ZOOM: %f\n", this->m_zoom);

    this->m_zoom = value;
    this->generate_view_matrix();
}

void MouseHandler::generate_view_matrix()
{
    printf("Zoom: %f\n", this->m_zoom);

    this->m_view_matrix = glm::translate(glm::mat4x4(1.0), glm::vec3(this->m_drag, 0.0));
    this->m_view_matrix = glm::scale(this->m_view_matrix, glm::vec3(this->m_zoom));
}
