#pragma once

#include "../Common.h"

class MouseHandler
{
public:
    MouseHandler()
    {
        printf("CONMSTRUCTOR\n");
        this->m_view_matrix = glm::mat4x4(1.0f);
        this->m_zoom = 1.0f;

        printf("SETTING ZOOM: %f\n", this->m_zoom);
    }

private:
    glm::vec2 m_pos;
    glm::vec2 m_drag;
    float m_zoom;

    glm::mat4x4 m_view_matrix;

    bool m_down;

    void generate_view_matrix();

public:
    void mouse_down(glm::vec2 pos);
    void mouse_move(glm::vec2 pos);
    void mouse_up(glm::vec2 pos);

    bool get_down();

    glm::vec2 get_drag();
    void set_drag(glm::vec2 drag);

    float get_zoom();
    void set_zoom(float value);

    glm::mat4x4 get_view_matrix();
};
