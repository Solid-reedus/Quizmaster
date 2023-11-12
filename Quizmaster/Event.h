#ifndef EVENT
#define EVENT

#ifndef BASE_HEADER
#include "BaseHeader.h"
#endif

#include <functional>
#include <algorithm>


class Event
{
	public:

    typedef std::function<void()> EventHandler;

    void operator+=(const EventHandler& handler);
	void Invoke();
	void InvokeCopy();
	void RemoveAt(size_t index);
	void Clear();

	private:
    std::vector<EventHandler> handlers;
};

#endif