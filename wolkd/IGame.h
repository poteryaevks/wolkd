#pragma once

#include <vector>
#include <memory>

#include "Types.h"
#include "IObject.h"
#include "IEventer.h"

namespace game
{
    class IGame : public IEventer
    {
    public:
        using Ptr = IGame*;
        virtual FRectRefs GetRects(ObjectsCategory) const noexcept = 0;
        virtual FVectType GetOffset() const noexcept = 0;
        virtual const FRectType& GetPlayerRect() const noexcept = 0;
        virtual IdType GetPlayerId() const noexcept = 0;
        virtual ~IGame() = default;
    };
}
