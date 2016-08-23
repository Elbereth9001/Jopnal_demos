#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <Jopnal/Jopnal.hpp>

class EventHandler : public jop::WindowEventHandler
{
public:
    EventHandler(jop::Window& window) :
        WindowEventHandler(window)
    {}

    void closed() override
    {
        jop::Engine::exit();
    }

    void keyPressed(const int key, const int, const int mods) override
    {
        using k = jop::Keyboard;

        if (key == k::Escape)
            closed();
    }

};

#endif