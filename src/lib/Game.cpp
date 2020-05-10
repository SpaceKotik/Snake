#include <climits>
#include <random>
#include <cstdlib>
#include <ctime>
#include "../headers/Game.hpp"
#include "../headers/Ui.hpp"
#include "../headers/Snake.hpp"


static std::random_device rseed;
static std::mt19937 rgen(rseed());


Game::Game() {
    Ui::get()->setTimer(TICK, std::bind(&Game::move, this));
}

bool Game::isOnField(Vector coord) {
    Ui *ui = Ui::get();

    return (coord.first != ui->getWidth() && coord.first != 1 &&
            coord.second != ui->getHeight() && coord.second != 1);
}

Vector Game::getRandomPoint() {
    srand(std::time(nullptr));
    //std::uniform_int_distribution<int> xdist(2, Ui::get()->get_width()  - 3);
    //std::uniform_int_distribution<int> ydist(2, Ui::get()->get_height() - 3);

    Vector xy;
    do {
        //xy = Vector(xdist(rgen), ydist(rgen));
        xy = Vector(2 + rand() % (Ui::get()->getWidth() - 3), 2 + rand() % (Ui::get()->getHeight() - 3));
    } while (!isOnField(xy));

    return xy;
}

void Game::add(Snake &snake) {
    snakes.emplace_back(snake);
}

void Game::draw() {
    Ui *ui = Ui::get();
    ui->drawAll();

    ///Draw snakes
    for (const Snake &sn : snakes) {
        SnakeState snakeDirection = sn.dir;
        for (const auto &elem : sn.body) {
            ui->draw({elem, sn.id, snakeDirection});
            snakeDirection = SnakeState::BODY;
        }
        ui->draw(sn.id, sn.body.size());
    }
    ///Draw rabbits
    for (const Rabbit &rab : rabbits)
        ui->draw(rab);
}

std::optional<Vector> Game::nearestRabbit(Vector pos) {
    std::optional<Vector> near;
    //TODO: repair controllers and change to smth like 5-10
    int distance = 1000;
    ///Find nearest rabbit which is now further than INT_MAX
    for (auto const &rab : rabbits) {
        int dist = rab.dist(pos);

        if (dist < distance) {
            distance = dist;
            near = rab;
        }
    }
    return near;
}

void Game::move() {
    Ui::get()->setTimer(TICK, std::bind(&Game::move, this));

    for (Snake &sn : snakes)
        sn.move(rabbits);

    ///Place new rabbit
    std::uniform_int_distribution<int> dice(1, 100 / SPAWN_CHANCE);
    if (rabbits.size() < MAX_RABBITS && dice(rgen) == 1) {
        auto newRabbitPos = getRandomPoint();
        rabbits.push_back(newRabbitPos);

        Ui::get()->draw(newRabbitPos);
    }
}
