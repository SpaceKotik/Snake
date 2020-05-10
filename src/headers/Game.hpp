#pragma once

#include <list>
#include "Structs.hpp"


class Snake;

class Game {
private:
    std::list<std::reference_wrapper<Snake>> snakes;
    std::list<Vector> rabbits;

public:
    enum {
        TICK = 100,
        MAX_RABBITS = 100,
        SPAWN_CHANCE = 20
    };

    Game();

    void move();

    void draw();

    void add(Snake &sn);

    static bool isOnField(Vector coord);

    static Vector getRandomPoint();

    std::optional<Vector> nearestRabbit(Vector pos);
};
