#pragma once

#include <memory>

#include "Types.h"
#include "Stats.h"

#include "../IInit.h"
#include "../IGame.h"
#include "../IObject.h"

#include <sgraphics/engine/IWindow.hpp>
#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/engine/IEventer.hpp>
#include <sgraphics/geometry/types.hpp>
#include <sgraphics/engine/ICollision.hpp>

namespace game
{
    class Player final : public IObject
    {
        friend class WolkingZombies;
    public:
        Player(IGame::Ptr game, PlayerStats &&stats);
        ~Player() override;

        void Show(const Duration &) noexcept override;
        const FRectType& GetRect() const noexcept override;
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        const std::size_t &GetWidth() const noexcept override;
        const std::size_t &GetHight() const noexcept override;
        void OnEvent(Event::Ptr msg) noexcept override;
        IdType GetId() const noexcept override;
        const Stats &GetStats() const noexcept;

    private:
        void DrawMe(const Duration &duration);
        void DrawStats();

    private:
        IGame::Ptr game_;
        PlayerStats stats_;
        sgraphics::ICollision::Ptr dynCollision_;
        sgraphics::ICollision::Ptr staticCollision_;
        sgraphics::IWindow::Ptr window_;
        sgraphics::IEventer::Ptr eventer_;
        FRectType rect_;
        sgraphics::ISprite::Ptr sprite_;
        FRectType offset_{};
        olc::vf2d mousePosition_{};
    };
}
