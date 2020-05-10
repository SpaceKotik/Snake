#include "Controllers.hpp"

#include "../headers/Ui.hpp"
#include "../headers/Game.hpp"

PlayerController::PlayerController(Game &game) : BaseController(game) {
    using namespace std::placeholders;
    Ui::get()->setEvent(std::bind(&PlayerController::onKeyPressed, this, _1));
}

void PlayerController::onKeyPressed(int key) {
    if (isDead)
        return;

    switch (key) {
        case Ui::KEY_UP:
            if (dir != SnakeState::HEAD_DOWN)
                dir = SnakeState::HEAD_UP;
            break;
        case Ui::KEY_DOWN:
            if (dir != SnakeState::HEAD_UP)
                dir = SnakeState::HEAD_DOWN;
            break;
        case Ui::KEY_RIGHT:
            if (dir != SnakeState::HEAD_LEFT)
                dir = SnakeState::HEAD_RIGHT;
            break;
        case Ui::KEY_LEFT:
            if (dir != SnakeState::HEAD_RIGHT)
                dir = SnakeState::HEAD_LEFT;
            break;
        default:
            break;
    }
}


AIController::AIController(Game &game) : BaseController(game) {
    Ui::get()->setTimer(Game::TICK, std::bind(&AIController::onTick, this));
}

void AIController::onTick() {
    if (isDead)
        return;

    Ui::get()->setTimer(Game::TICK, std::bind(&AIController::onTick, this));

    SnakeState d[] = {SnakeState::HEAD_UP, SnakeState::HEAD_DOWN, SnakeState::HEAD_LEFT, SnakeState::HEAD_RIGHT};

    auto head = body.front();
    auto rabbit = game.nearestRabbit(head);

    if (rabbit.has_value()) {
        for (int i = 0; i < 4; ++i) {
            dir = d[i];
            if (!checkIfLost(this->next()) && rabbit->dist(this->next()) <= rabbit->dist(head))
                return;
        }
    }

    for (int i = 0; i < 4; ++i) {
        dir = d[i];
        if (!checkIfLost(this->next()))
            return;
    }
}
