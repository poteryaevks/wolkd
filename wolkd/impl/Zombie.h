#pragma once

#include <optional>
#include <memory>

#include "Types.h"
#include "Stats.h"
#include "IGame.h"
#include "IObject.h"

#include <sgraphics/engine/ICollision.hpp>
#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/engine/IRenderer.hpp>

namespace game
{
    class Zombie final : public IObject
    {
    public:
        enum State
        {
            TO_PLAYER,
            SHUFFLE
        };

        Zombie(IGame::Ptr game, ZombieStats &&stats);
        ~Zombie() override;
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        void Show(const Duration &) noexcept override;
        const std::size_t &GetWidth() const noexcept override;
        const std::size_t &GetHight() const noexcept override;
        const FRectType &GetRect() const noexcept override;
        void OnEvent(Event::Ptr event) noexcept override;
        IdType GetId() const noexcept override;
        const Stats &GetStats() const noexcept;

    private:
        State UpdateState(const FRectType &player);
        void DrawMe(const Duration &);
        void Shuffle(const Duration &);
        void Collide(const Duration &, State);
        void Attack(const Duration &);

    private:
        IGame::Ptr game_;
        sgraphics::IRenderer::Ptr renderer_;
        FRectType realRect_;
        FRectType rect_;
        ZombieStats stats_;
        sgraphics::ICollision::Ptr dynCollision_;
        sgraphics::ICollision::Ptr staticCollision_;
        sgraphics::ISprite::Ptr sprite_;
        State prevState_;
    };
}
