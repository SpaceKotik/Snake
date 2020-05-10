#include <climits>
#include <random>
#include "../headers/Ui.hpp"
#include "../headers/Game.hpp"
#include "../headers/Snake.hpp"


Snake::Snake(Game &game) : isDead(false), dir(SnakeState::HEAD_RIGHT), game(game) {
    static int nextID = 1;
    id = nextID++;
    body.push_back(game.getRandomPoint());
    Ui::get()->draw(id, body.size());
}

Vector Snake::next() {
    Vector head = body.front();
    switch (dir) {
        case HEAD_UP:
            --head.second;
            break;
        case HEAD_DOWN:
            ++head.second;
            break;
        case HEAD_RIGHT:
            ++head.first;
            break;
        case HEAD_LEFT:
            --head.first;
            break;
        default:
            break;
    }
    return head;
}

void Snake::move(std::list<Vector> &rabbits) {
    if (isDead)
        return;

    Vector tail = this->body.back();
    Vector head = this->body.front();
    Vector next = this->next();

    Ui *ui = Ui::get();
    ///Check if bumped into a wall
    if (checkIfLost(next)) {
        isDead = true;
        return;
    }

    body.push_front(next);

    ui->draw({next, id, dir});
    if (tail != head)
        ui->draw({head, id, SnakeState::BODY});

    auto i = std::find(rabbits.begin(), rabbits.end(), next);
    ///If not on the rabbit
    if (i == rabbits.end()) {
        body.pop_back();
        ui->draw({tail, id, SnakeState::NONE});
    } else {
        rabbits.erase(i);
        ui->draw(id, body.size());
    }
}

bool Snake::checkIfLost(Vector next) {
    ///Check borders
    if (!Game::isOnField(next)) {
        return true;
    }
    ///Check body
    for (auto e : body) {
        if (e.first == next.first && e.second == next.second) {
            return true;
        }
    }
    return false;
}
