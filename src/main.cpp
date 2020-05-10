#include <memory>
#include <iostream>
#include "headers/Game.hpp"
#include "headers/Ui.hpp"
#include "Controllers.hpp"

int main() {

    std::unique_ptr<Ui> ui(Ui::get("text"));
    if (!ui) {
        std::cout << "UI is not recognized\n";
        return 0;
    }

    Game game;
    PlayerController player(game);
    AIController computer(game);
    ui->run(game);
}
