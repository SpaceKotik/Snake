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

    explicit Snake(Game& game);

    Snake(const Snake& snake) = delete;

    Snake &operator=(const Snake& snake) = delete;

    Vector next();

    void move(std::list<Vector> &rabbits);

    bool checkIfLost(Vector next);
};
