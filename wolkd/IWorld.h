#pragma once

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/geometry/olc.h>

#include "Types.h"

#include <IEventer.h>

namespace game
{
    class IWorld : public IEventer
    {
    public:
        using Ptr = std::shared_ptr<IWorld>;
        virtual std::vector<FRectPtr> GetRects() const noexcept = 0;
        virtual void Show(const Duration& duration) noexcept = 0;
        virtual std::size_t GetWidth() const noexcept = 0;
        virtual std::size_t GetHight() const noexcept = 0;
        virtual ~IWorld() = default;
    };
}
