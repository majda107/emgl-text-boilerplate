#pragma once

#include "..\Common.h"

#include "MouseHandler.h"

class StateHandler
{
public:
    MouseHandler mouse_handler;

    StateHandler()
    {
        printf("State handler constructor!\n");
    }
};