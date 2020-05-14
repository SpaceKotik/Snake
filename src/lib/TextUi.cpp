#include <cstdio>
#include <sys/poll.h>
#include "../headers/TextUi.hpp"
#include "../headers/Game.hpp"


void TextUi::onWindowResized(int sig) {
    Ui::get()->drawAll();
}

void TextUi::onQuit(int sig) {
    Ui::get()->quit();
}

TextUi::TextUi() {
    ///Set new settings for terminal driver
    ///"in" buffer size set to 1
    ///Set terminal accepting input after each char
    struct termios newTerminalSettings;

    tcgetattr(0, &newTerminalSettings);
    terminalInfo = newTerminalSettings;

    newTerminalSettings.c_lflag &= ~ICANON;
    newTerminalSettings.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminalSettings);


    ///On user quit
    signal(SIGINT, onQuit);
    ///On finishing process
    signal(SIGTERM, onQuit);
    ///On resizing terminal
    signal(SIGWINCH, onWindowResized);

    updateWindowSize();
    ///buffer is not being used
    setvbuf(stdout, nullptr, _IONBF, 0);
}

TextUi::~TextUi() {
    clearWindow();
    resetSignals();
    setvbuf(stdout, nullptr, _IOLBF, 0);
    printf("Well played :)\n");
}

void TextUi::resetSignals() {
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalInfo);
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGWINCH, SIG_DFL);
}

void TextUi::clearWindow() {
    printf("\e[H\e[J\e[m");
}

void TextUi::gotoXY(int x, int y) {
    ///ANSI escape sequence
    printf("\e[%d;%dH", getHeight() - y, x + 1);
}

void TextUi::setColor(int color) {
    printf("\e[%dm", color ? color + 30 : 0);
}

void TextUi::putChar(int x, int y, char c) {
    printf("\e[%d;%dH%c", y, x, c);
}

void TextUi::draw(int id, int score) {

    gotoXY(10 * id, -1);
    setColor(id);
    printf("%d", score);
    setColor(0);
    gotoXY(0, 0);
}

void TextUi::draw(const BodyPart &elem) {
    setColor(elem.id);
    putChar(elem.first, elem.second, "^v><# "[elem.dir]);
    if (elem.dir == BODY)
        //putChar(elem.first, elem.second, -2);
        putChar(elem.first, elem.second, 219);
    setColor(0);
    gotoXY(0, 0);
}

void TextUi::draw(const Rabbit &rab) {
    putChar(rab.first, rab.second, -2);
    gotoXY(0, 0);
}

void TextUi::updateWindowSize() {
    ioctl(STDIN_FILENO, TIOCGWINSZ, &windowInfo);
}

void TextUi::quit() {
    running = false;
}

void TextUi::setEvent(EventFunc eventFunc) {
    events.push_back(eventFunc);
}

void TextUi::setTimer(int time, TimerFunc timerFunc) {
    timers.emplace_back(std::max(0, time), std::max(0, time), timerFunc);
    ///Sort times from smallest to biggest
    std::sort(timers.begin(), timers.end(),
              [](const auto &a, const auto &b) {
                  return std::get<0>(a) < std::get<0>(b);
    });
}

void TextUi::drawAll() {
    updateWindowSize();
    clearWindow();

    ///Draw horizontal borders
    for (int i = 1; i <= getWidth(); i++) {
        putChar(i, 1, 219);
        putChar(i, getHeight(), 219);
    }
    ///Draw vertical borders
    for (int i = 1; i <= getHeight(); i++) {
        putChar(1, i, 219);
        putChar(getWidth(), i, 219);
    }

    ///Print score title
    gotoXY(0, -1);
    printf(" Score: ");

    ///Reset cursor position
    gotoXY(0, 0);
}

void TextUi::run(Game &game) {
    game.draw();

    running = true;
    while (running) {
        struct pollfd pfd[1] = {{.fd = STDIN_FILENO, .events = POLLIN}};

        int minSecs = -1;
        if (!timers.empty())
            minSecs = std::get<0>(timers.front());

        struct timespec ta, tb;
        clock_gettime(CLOCK_MONOTONIC, &ta);
        ///Wait for minSecs and check if events happened
        int eventsFound = poll(pfd, 1, minSecs);
        clock_gettime(CLOCK_MONOTONIC, &tb);
        int elapsed = ((tb.tv_sec - ta.tv_sec) * 1000 + (tb.tv_nsec - ta.tv_nsec) / 1000000);

        ///If there are some subscribers in timers
        if (minSecs != -1) {
            ///Reduce time in every timer
            for (auto &t : timers) {
                std::get<0>(t) = std::max(0, std::get<0>(t) - elapsed);
            }
            ///If the timer's event must have happened, erase it and make call event
            for (auto &t : timers) {
                if (std::get<0>(t) <= 0) {
                    std::get<0>(t) = std::get<1>(t);
                    std::get<2>(t)();
                }
            }

        }

        if (eventsFound == 1)
            readEvent();
    }
}

bool TextUi::readEvent() {
    char ch[100];
    int n = read(STDIN_FILENO, ch, sizeof(ch));
    ///If smth went wrong or user pressed esc
    if (n <= 0 || (n == 1 && ch[0] == '\e')) {
        quit();
        return false;
    }

    for (int i = 0; i < n; i++) {
        int key = ch[i];

        if (ch[i] == '\e' && i + 2 < n && ch[i + 1] == '[') {
            i += 2;
            switch (ch[i]) {
                case 'A':
                    key = KEY_UP;
                    break;
                case 'B':
                    key = KEY_DOWN;
                    break;
                case 'C':
                    key = KEY_RIGHT;
                    break;
                case 'D':
                    key = KEY_LEFT;
                    break;
            }
        }
        ///On key pressed trigger all events
        for (auto &event : events)
            event(key);
    }

    return true;
}
