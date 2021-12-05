// Copyright 2021 Marko Vejnovic

#include "event.hpp"

int EventBuffer::registerEvent(Event* event) {
    this->eventMap.insert({ event->trigger, event });

    return EVENT_BUFFER_OK;
}

void EventBuffer::trigger(std::string trigger, EventMessage* evMsg) {
    auto events = this->eventMap.equal_range(trigger);

    for (auto it = events.first; it != events.second; it++) {
        Event* event = it->second;
        event->shot(evMsg);
    }
}

EventBuffer::~EventBuffer() {
    for (auto it = this->eventMap.begin(); it != this->eventMap.end(); it++) {
        delete it->second;
    }
}
