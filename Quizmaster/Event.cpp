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
            //std::cout << "Invoking handler: " << typeid(handler).name() << std::endl;
            handler();
        }
    }
}

void Event::Clear()
{
    handlers.clear();
}
