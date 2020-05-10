#pragma once

#include <optional>
#include <cmath>

enum SnakeState {
    HEAD_UP,
    HEAD_DOWN,
    HEAD_RIGHT,
    HEAD_LEFT,
    BODY,
    NONE,
};


struct Vector : public std::pair<int, int> {
    using Parent = std::pair<int, int>;
    using Parent::Parent;

    int dist(const Vector &c) const {
        return std::abs(this->first - c.first) + std::abs(this->second - c.second);
    }
};

struct BodyPart : Vector {
    BodyPart(const Vector &coord, int id, SnakeState snakeDir) : Vector(coord), id(id), dir(snakeDir) {}
    int id;
    SnakeState dir;
};

struct Rabbit : Vector, std::optional<int> {
    Rabbit(const Vector &coord) : Vector(coord) {}
};
