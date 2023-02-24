#pragma once

#include "Types.h"
#include <sgraphics/engine/ICollision.hpp>

namespace
{
    template <typename From, typename To>
    To Convert(const From &from)
    {
        return To();
    }

    template <>
    sgraphics::ICollision::RectsType Convert<game::FRectRefs, sgraphics::ICollision::RectsType>(const game::FRectRefs &rects)
    {
        sgraphics::ICollision::RectsType result;
        for (const auto &rect : rects)
        {
            result.push_back(const_cast<game::FRectType *>(&(rect.get())));
        }

        return result;
    }
}
