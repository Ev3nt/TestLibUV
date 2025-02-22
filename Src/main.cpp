#include <uvw.hpp>

#include <iostream>
#include "Timer.h"
#include "ByteBuffer.h"


int main() {
    ByteBuffer t;
    t << (char)1;
    t << (char)1;
    int a;
    t >> a;

    std::cout << (int)a << std::endl;

    auto loop = uvw::loop::get_default();
    auto timer = Timer::create([]() {
        std::cout << "First timer!" << std::endl;
        }, *loop);
    timer->start(std::chrono::seconds(1));

    auto timer2 = Timer::create([](Timer& timer) {
        std::cout << timer.getActiveContainersCount() << std::endl;
        timer.close();
        std::cout << "Closed! " << timer.getContainersCount() << std::endl;
        }, *loop);
    timer->attach(timer2);
    timer2->start(std::chrono::seconds(2));

    auto timer3 = Timer::create([]() {
        std::cout << "I'm alive!" << std::endl;
        }, *loop);
    timer3->start(std::chrono::milliseconds(500));
    timer2->attach(timer3);
    timer3->setCloseCallback([]() {
        std::cout << "I was closed :(" << std::endl;
        });

    auto timer4 = Timer::create([]() {
        std::cout << "Fourth timer working!" << std::endl;
        }, *loop);
    timer4->start(std::chrono::milliseconds(200));
    timer3->attach(timer4);

    std::cout << "Running..." << std::endl;
    loop->run();
    std::cout << "Program end" << std::endl;
}