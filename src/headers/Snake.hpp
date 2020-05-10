#pragma once

#include <cstdio>
#include <list>
#include "Structs.hpp"

class Game;

class Snake {
private:
    Game &game;
public:
    SnakeState dir;
    int id;
    std::list<Vector> body;
    bool isDead;

    explicit Snake(Game &);

    Snake(const Snake &) = delete;

    Snake &operator=(const Snake &) = delete;

    Vector next();

    void move(std::list<Vector> &rabbits);

    bool checkIfLost(Vector next);
};
