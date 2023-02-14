#pragma once

#include <boost/noncopyable.hpp>
#include <vector>
#include <memory>

#include "Types.h"

namespace game
{
    class ICollision : private boost::noncopyable
    {
    public:
        using Ptr = std::shared_ptr<ICollision>;
        using RectsType = std::vector<FRectPtr>;
        virtual RectsType GetContacts() const = 0;
        virtual bool Calculate(FRectType &rect, const RectsType &rects, float time) = 0;
        virtual ~ICollision() = default;
    };
}
