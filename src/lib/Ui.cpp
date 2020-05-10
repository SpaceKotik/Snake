#include "../headers/Ui.hpp"
#include "../headers/TextUi.hpp"
#include <cstring>


Ui *Ui::instance = nullptr;

Ui *Ui::get(const char *ui_type) {
    if (ui_type && !instance) {
        if (!strcmp(ui_type, "text"))
            instance = new TextUi();
    }
    return instance;
}

Ui::~Ui() {
    instance = nullptr;
}
