#pragma once

#include <termios.h>
#include <sys/ioctl.h>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include "Ui.hpp"

class TextUi : public Ui {
private:
    struct winsize windowInfo;
    struct termios terminalInfo;
    bool running = true;

    std::vector<EventFunc> events;
    std::vector<std::pair<int, TimerFunc>> timers;

    static void clearWindow();                       ///Clears window
    static void putChar(int x, int y, char c);
    static void setColor(int color);
    void gotoXY(int x, int y);
    bool readEvent();
    void resetSignals();
    void updateWindowSize();

public:
    TextUi();
   ~TextUi();

    void run(Game&) override;
    void quit() override;

    void setEvent(EventFunc eventFunc) override;
    void setTimer(int time, TimerFunc timerFunc) override;

    void draw(const BodyPart&) override;
    void draw(const Rabbit&) override;
    void draw(int id, int score) override;
    void drawAll() override;

    static void onWindowResized(int sig);
    static void onQuit(int sig);

    inline int getWidth() const override {
        return windowInfo.ws_col;
    }
    inline int getHeight() const override {
        return windowInfo.ws_row - 1;
    }

};
