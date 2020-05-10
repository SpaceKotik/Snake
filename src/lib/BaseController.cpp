#include "BaseController.hpp"
#include "../headers/Game.hpp"


BaseController::BaseController(Game& game) : Snake(game), game(game) {
    game.add(*this);
}
