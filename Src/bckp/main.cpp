#include <uvw.hpp>

#include <iostream>

#include "Timer.h"

class MyVal {
public:
    MyVal(int64_t val) : val(val) {
        std::cout << "Created!" << std::endl;
    }

    ~MyVal() {
        std::cout << "Deleted!" << std::endl;
    }

    int64_t& operator*() {
        return val;
    }

    void print(Timer& timer) {
        std::cout << val << std::endl;
    }

private:
    int64_t val;
};

void idle(uvw::loop& loop) {
    auto idle = loop.resource<uvw::idle_handle>();
    auto counter = std::make_shared<MyVal>(0);

    idle->on<uvw::idle_event>([counter](const uvw::idle_event, uvw::idle_handle& idle) {
        (**counter)++;

        if (**counter >= 10e6) {
            //idle.stop();
            idle.close();
        }

        });

    idle->start();
}

int main() {
    auto loop = uvw::loop::get_default();

    MyVal val(1);
    Timer timer([&val](Timer& timer) {
        val.print(timer);
        timer.stop();
        timer.close();
        }, *loop);
    timer.start(std::chrono::seconds(2), Timer::timer_mode::DEFAULT);

    Timer timer2([](Timer& timer) {
        std::cout << "timer2!" << std::endl;
        }, *loop);

    timer.attach(&timer2);
    timer2.start(std::chrono::milliseconds(500), Timer::timer_mode::DEFAULT);

    std::cout << "Running..." << std::endl;
    loop->run();
    std::cout << "Program end" << std::endl;
}