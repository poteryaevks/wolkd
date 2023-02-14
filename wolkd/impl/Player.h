#pragma once

#include <memory>

#include "Types.h"
#include "CollisionImpl.h"
#include "Stats.h"

#include "../IInit.h"
#include "../IGame.h"
#include "../IObject.h"
#include "../ICollision.h"

#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/geometry/types.hpp>

namespace game
{
    class Player final : public IObject
    {
        enum State
        {
            MOVING,
            STAYING
        };

    public:
        Player(IGame::Ptr game, Stats::Ptr initData);
        ~Player() override;

        void Show(const Duration &) noexcept override;
        FRectPtr GetRect() const noexcept;
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        std::size_t GetWidth() const noexcept override;
        std::size_t GetHight() const noexcept override;
        void OnAttack(Event::Ptr msg) noexcept override;
        void OnInput(sg::MousePosType xy) noexcept override;
        IdType GetId() const noexcept override;
        Stats::Ptr GetStats() const noexcept;

    private:
        void DrawMe(const Duration &duration);
        void DrawStats();

    private:
        IGame::Ptr game_;
        Stats::Ptr stats_;
        ICollision::Ptr checkCollision_;
        sg::ISprite::Ptr sprite_;
        std::uint32_t count_{};
        Duration updateSprite_{};
        FRectType realRect_;
        FRectType screenRect_;
        PointType mouseOffset_;
        eOrientation orientation_{eOrientation::RIGHT};
        float FPS_;
    };
}
