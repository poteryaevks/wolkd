#pragma once

#include <boost/noncopyable.hpp>

#include "iobject.h"

namespace zgame {


//!
//! \brief The IInit class
//!
class IInit : private boost::noncopyable
{

public:

    //!
    //! \brief Деструктор
    //!
    virtual ~IInit() = default;

    //!
    //! \brief init
    //! \param object
    //!
    virtual void init(IObject& object) = 0;

protected:

    IInit() = default;
};


}
