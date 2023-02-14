#pragma once

#include <memory>

#include "Types.h"
#include "CollisionImpl.h"
#include "Stats.h"
#include "IGame.h"
#include "IObject.h"
#include "ICollision.h"

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
        Zombie(IGame::Ptr game, Stats::Ptr stats);
        ~Zombie();

    public:
        CollisionType GetCollisionType() const noexcept override;
        eObjectCategory GetCategory() const noexcept override;
        void Show(const Duration &) noexcept override;
        std::size_t GetWidth() const noexcept override;
        std::size_t GetHight() const noexcept override;
        FRectPtr GetRect() const noexcept override;
        void OnAttack(Event::Ptr event) noexcept override;
        IdType GetId() const noexcept override;
        Stats::Ptr GetStats() const noexcept;
        void OnInput(sg::MousePosType xy) noexcept override;

    private:
        PointType GetTarget();
        void DrawMe();
        void CollisionsExec(const Duration &);
        void Attack(IdType id);

    private:
        IGame::Ptr game_;
        FRectType rect_;
        Stats::Ptr stats_;
        ZombieStats& zombieStats_;
        ICollision::Ptr checkCollision_;
        Duration elapsedTime_{};
        Duration elapsedFromAttack_{};
        std::uint32_t count_{};
        eOrientation orientation_{eOrientation::RIGHT};
        State state_{SHAFFLE};
        sg::ISprite::Ptr sprite_;
    };
}
