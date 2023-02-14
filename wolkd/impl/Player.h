#pragma once

#include <memory>

#include "Types.h"
#include "Stats.h"

#include "../IInit.h"
#include "../IGame.h"
#include "../IObject.h"

#include <sgraphics/engine/IWindow.hpp>
#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/geometry/types.hpp>
#include <sgraphics/engine/ICollision.hpp>

namespace game
{
    class Player final : public IObject
    {
    public:
        Player(IGame::Ptr game, PlayerStats &&stats);
        ~Player() override;

        void Show(const Duration &) noexcept override;
        const FRectType& GetRect() noexcept;
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        const std::size_t &GetWidth() const noexcept override;
        const std::size_t &GetHight() const noexcept override;
        void OnEvent(Event::Ptr msg) noexcept override;
        void OnInput(sg::MousePosType xy) noexcept override;
        IdType GetId() const noexcept override;
        const Stats &GetStats() const noexcept;

    private:
        void DrawMe(const Duration &duration);
        void DrawStats();

    private:
        IGame::Ptr game_;
        PlayerStats stats_;
        sg::ICollision::Ptr checkCollision_;
        sg::IWindow::Ptr window_;
        FRectType rect_;
        sg::ISprite::Ptr sprite_;
        FRectType offset_{};
        PointType mousePosition_{};
    };
}
