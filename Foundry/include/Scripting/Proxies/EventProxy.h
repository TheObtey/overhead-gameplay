#ifndef FOUNDRY_EVENT_PROXY__H_
#define FOUNDRY_EVENT_PROXY__H_

#include "Scripting/Binder.h"
#include "Scripting/Proxies/NodeProxy.inl"

// template <typename T>
// class EventProxy
// {
// public:
//     EventProxy(Event<T> event) : m_event(event) {}
//
//
// private:
//     Event<T>& m_event;
// };

struct EventProxyBinding
{
    static void Bind(Binder& binder);
};

REGISTER_PROXY(EventProxyBinding, EventProxy);

#endif