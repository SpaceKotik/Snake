#pragma once

#include "Snake.hpp"

class BaseController : public Snake {
protected:
    Game& game;
public:
    BaseController(Game& game);
};
