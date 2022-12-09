#pragma once

#include <list>
#include <map>
#include <vector>

#include <boost/noncopyable.hpp>


#include "ztypes.h"

namespace math { class Graph; }

//!
namespace zgame{

class IObject;

//!
//! \brief The IGame class
//!
class IGame : private boost::noncopyable
{

public:

    //!
    //! \brief getObjects
    //! \return
    //!
    virtual std::vector<std::shared_ptr<zgame::rect>> getRects(ObjectsCategory) const noexcept = 0;

    //!
    //! \brief getGraph
    //! \return
    //!
    virtual math::Graph getGraph() noexcept = 0;

    //!
    //! \brief getOffset
    //!
    virtual zgame::vect_type getOffset() const noexcept = 0;

    //!
    //! \brief getPlayerPos
    //! \return
    //!
    virtual zgame::point_type getPlayerPos() const noexcept = 0;

    //!
    //! \brief getPlayerPos
    //! \return
    //!
    virtual zgame::id_type getPlayerId() const noexcept = 0;

};

}

