// Copyright 2021 Marko Vejnovic

#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <unordered_map>
#include <string>
#include <vector>

#define EVENT_BUFFER_OK 0
#define EVENT_BUFFER_ERR_TAKEN 1

class EventMessage {
};

class Event {
 public:
    virtual int shot(EventMessage *) { return -1; }
    virtual ~Event();
    std::string trigger;
};

/**
 * A class for keeping track of events. Simple hash-map under-the-hood.
 */
class EventBuffer {
 public:
    /**
     * Registers an Event for event handling.
     */
    int registerEvent(Event*);

    /**
     * Called to signify that an event is shot. All registered events will
     * handle the event.
     */
    void trigger(std::string, EventMessage*);

    /**
     * Cleans up the registered events. After this point they are unusable.
     * Potentially very pricy. O(n)
     */
    ~EventBuffer();
 private:
    std::unordered_multimap<std::string, Event*> eventMap;
};

#endif  // _EVENT_HPP_
