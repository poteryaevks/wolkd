#pragma once

#include <list>
#include <map>
#include <vector>

#include <boost/noncopyable.hpp>

#include <sgraphics/engine/isprite.h>
#include <sgraphics/geometry/olc.h>

#include "ztypes.h"
#include "zproto.h"

//!
namespace zgame{

//!
//! \brief The IWorld class
//!
class IWorld
{

public:

    //!
    //! \brief getRects
    //! \return
    //!
    virtual std::vector<std::shared_ptr<zgame::rect>> getRects() const noexcept = 0;

    //!
    //! \brief show
    //! \param fElapsedTime
    //!
    virtual void show(float fElapsedTime) noexcept = 0;

    //!
    //! \brief getWidthImpl
    //!
    virtual std::size_t getWidth() const noexcept = 0;

    //!
    //! \brief getHightImpl
    //!
    virtual std::size_t getHight() const noexcept = 0;

    virtual ~IWorld() = default;

protected:

    IWorld() = default;
};

#include "ieventer.h"

//!
//! \brief The BaseWorld class
//!
class BaseWorld : public IEventer<std::shared_ptr<zgame::Event>>,
        public zgame::IWorld
{

public:

    virtual ~BaseWorld() = default;

protected:

    BaseWorld() = default;

    BaseWorld(std::uint32_t id)
        : IEventer<std::shared_ptr<zgame::Event>>(),
          zgame::IWorld()
    {}

};

}

