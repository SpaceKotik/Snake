#pragma once

#include <functional>
#include "Structs.hpp"


class Game;

class Ui {
private:
    static Ui *instance;
protected:
    Ui() = default;

public:
    using EventFunc = std::function<void(int)>; ///Subscriber for key
    using TimerFunc = std::function<void(void)>; ///Subscriber for timer

    virtual ~Ui() = 0;

    enum {
        KEY_UP = 1000,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
    };

    static Ui *get(const char *ui_type = nullptr);

    virtual void run(Game &) = 0;

    virtual void quit() = 0;


    virtual void setEvent(EventFunc fn) = 0;        ///Add subscriber for events

    virtual void setTimer(int, TimerFunc fn) = 0;   ///Add subscriber for timers

    virtual void draw(const BodyPart &) = 0;

    virtual void draw(const Rabbit &) = 0;

    virtual void draw(int id, int score) = 0;

    virtual void drawAll() = 0;


    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

};
