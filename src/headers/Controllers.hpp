#pragma once

#include "BaseController.hpp"

class PlayerController : public BaseController {
public:
    PlayerController(Game &game);

    void onKeyPressed(int key);
};

class AIController : public BaseController {
public:
    AIController(Game &game);

    void onTick();
};
