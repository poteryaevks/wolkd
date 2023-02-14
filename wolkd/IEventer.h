#pragma once

#include <alm/common/event.h>
#include <boost/noncopyable.hpp>
#include <Event.h>

namespace game
{
    class IEventer : private boost::noncopyable
    {
    public:
        virtual ~IEventer() = default;

        template <class Object, class... TParams>
        void addEventHandler(Object &object, void (Object::*method)(TParams...))
        {
            eventer_ += METHOD_HANDLER(object, method);
        }

    protected:
        IEventer() = default;
        alm::common::TEvent<Event::Ptr> eventer_;
    };
}
