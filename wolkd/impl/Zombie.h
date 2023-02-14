#pragma once

#include <memory>

#include "Types.h"
#include "Stats.h"
#include "IGame.h"
#include "IObject.h"

#include <sgraphics/engine/ICollision.hpp>
#include <sgraphics/engine/ISprite.hpp>

namespace game
{
    class Zombie final : public IObject
    {
        enum State
        {
            TO_PLAYER,
            STAYING,
            SHAFFLE,
            ATTACK
        };

    public:
        Zombie(IGame::Ptr game, ZombieStats &&stats);
        ~Zombie() override;

    public:
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        void Show(const Duration &) noexcept override;
        const std::size_t &GetWidth() const noexcept override;
        const std::size_t &GetHight() const noexcept override;
        const FRectType& GetRect() noexcept override;
        void OnEvent(Event::Ptr event) noexcept override;
        IdType GetId() const noexcept override;
        const Stats &GetStats() const noexcept;
        void OnInput(sg::MousePosType xy) noexcept override;

    private:
        PointType GetTarget();
        void DrawMe();
        void CollisionsExec(const Duration &);
        void Attack(IdType id);

    private:
        IGame::Ptr game_;
        FRectType rect_;
        ZombieStats stats_;
        sg::ICollision::Ptr checkCollision_;
        Duration elapsedTime_{};
        Duration elapsedFromAttack_{};
        std::uint32_t count_{};
        eOrientation orientation_{eOrientation::RIGHT};
        State state_{SHAFFLE};
        sg::ISprite::Ptr sprite_;
    };
}
