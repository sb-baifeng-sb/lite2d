#include "event.hpp"
#include "world.hpp"

namespace entity
{

BaseEvent::Id BaseEvent::idCounter = 0;

void EventManager::DestroyEvents()
{
    for (auto &it : eventPools) {
        auto pool = it.second;
        pool->Clear();
    }
}

}
