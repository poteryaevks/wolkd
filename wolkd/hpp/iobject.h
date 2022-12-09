#pragma once

#include <list>
#include <map>
#include <vector>

#include <boost/noncopyable.hpp>

#include <sgraphics/engine/isprite.h>
#include <sgraphics/geometry/olc.h>

#include "ztypes.h"
#include "init_types.h"
#include "zproto.h"


//!
namespace zgame{

//!
//! \brief The IObject class
//!
class IObject : private boost::noncopyable
{

public:

    //!
    //! \brief getCollisionType
    //! \return
    //!
    virtual eObjectCollisionType getCollisionType() const noexcept = 0;

    //!
    //! \brief getCategory
    //! \return
    //!
    virtual eObjectCategory getCategory() const noexcept = 0;

    //!
    //! \brief getRects
    //! \return
    //!
    virtual std::shared_ptr<zgame::rect> getRect() const noexcept = 0;

    //!
    //! \brief show
    //! \param fElapsedTime
    //!
    virtual void show(float fElapsedTime) noexcept = 0;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getWidth() const noexcept = 0;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getHight() const noexcept = 0;

    //!
    //! \brief attack
    //! \param damage
    //!
    virtual void getAttacked(std::shared_ptr<zgame::Event> msg) noexcept = 0;

    //!
    //! \brief getId
    //! \return
    //!
    virtual zgame::id_type getId() const noexcept = 0;

    //!
    //! \brief getStats
    //! \return
    //!
    virtual std::shared_ptr<zgame::Stats> getStats() const noexcept = 0;
};


#include "ieventer.h"


//!
//! \brief The BaseObject class
//!
class BaseObject : public IEventer<std::shared_ptr<zgame::Event>>,
        public zgame::IObject
{

public:

    virtual ~BaseObject() = default;

protected:

    BaseObject() = default;

    BaseObject(std::uint32_t id)
        : IEventer<std::shared_ptr<zgame::Event>>(),
          zgame::IObject()
    {}

};

} //zgame

