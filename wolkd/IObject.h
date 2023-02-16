#pragma once

#include <memory>

#include "Types.h"
#include "Stats.h"
#include "IEventer.h"

#include <sgraphics/geometry/types.hpp>

namespace game
{
    class IObject : public game::IEventer
    {
    public:
        using Ptr = std::shared_ptr<IObject>;
        virtual CollisionType GetCollisionType() const noexcept = 0;
        virtual eObjectCategory GetCategory() const noexcept = 0;
        virtual const FRectType& GetRect() const noexcept = 0;
        virtual void Show(const Duration &) noexcept = 0;
        virtual const std::size_t &GetWidth() const noexcept = 0;
        virtual const std::size_t &GetHight() const noexcept = 0;
        virtual void OnInput(sg::MousePosType xy) noexcept = 0;
        virtual void OnEvent(Event::Ptr) noexcept = 0;
        virtual IdType GetId() const noexcept = 0;
        virtual const Stats &GetStats() const noexcept = 0;
        virtual ~IObject() = default;
    };
} // game
