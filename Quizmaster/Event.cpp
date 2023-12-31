#include "Event.h"


void Event::operator+=(const EventHandler& handler) 
{
    handlers.push_back(handler);
}


void Event::Invoke()
{
    for (const auto& handler : handlers)
    {
        if (handler)
        {
            handler();
        }
    }
}

void Event::InvokeCopy()
{
    std::vector<EventHandler> h = handlers;
    for (const auto& handler : h)
    {
        if (handler)
        {
            handler();
        }
    }
}

void Event::Clear()
{
    handlers.clear();
}
