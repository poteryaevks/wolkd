#pragma once

#include <alm/common/event.h>
#include <boost/noncopyable.hpp>

#include <alm/common/event.h>

//!
//! \brief The IEventer class интерфейс, предоставляющий функционал для генерации событий
//!  \author Потеряев К.С.
//!
template <class EventType>
class IEventer : private boost::noncopyable
{
    using eventer_type = alm::common::TEvent<EventType>;

public:

    //!
    //! \brief Деструктор
    //!
    virtual ~IEventer() = default;

    //!
    //! \brief Возвращает объект-событие. Необходимо для оформление подписки
    //! \return Объект-событие
    //!
    template<class Object, class ...TParams>
    void addEventHandler(Object& object, void(Object::*method)(TParams...))
    {
        // оформляем подписку на события
        eventer_ += METHOD_HANDLER(object, method);
    }


protected:

    IEventer() = default;

    eventer_type eventer_;  //!< Функциональны объект. Генерирует события типа EventType и отправляет их всем, кто на них подписан.
};
