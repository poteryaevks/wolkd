#include "Zombie.h"
#include "impl/CollisionImpl.h"
#include "../Types.h"

#include <iostream>
#include <math.h>

#include <alm/misc/random.h>
#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/Engine.hpp>

namespace
{
    using namespace game;
    using namespace std::chrono_literals;

    const PointType NO_TARGET({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()});
    const PointType SPRITE_SIZE{50, 50};
    constexpr int PNG_SIZE{32};

    FRectType getSrcRect(ZombieStats::eZombieType type, eOrientation orient, std::uint32_t count)
    {
        std::uint32_t xIndex{type - (type / 4) * 4};
        std::uint32_t yIndex{type / 4};

        return FRectType({{(((std::int32_t)count % 3) * PNG_SIZE) + PNG_SIZE * 3 * xIndex, (orient * PNG_SIZE) + PNG_SIZE * 4 * yIndex},
                          {PNG_SIZE, PNG_SIZE}});
    }

    template <class T>
    void speedUp(olc::v2d_generic<T> &vel, const ZombieStats &data)
    {
        vel = vel.norm() * data.speed;
    }

    template <class T>
    void speedDown(olc::v2d_generic<T> &vel, const ZombieStats &data)
    {
        vel = {data.speed, 0};
    }

    FRectType getOffsetted(const FRectType &rect, FVectType offset)
    {
        FRectType offseted(rect);
        offseted.pos += offset;
        return offseted;
    }

}

namespace game
{
    Zombie::Zombie(IGame::Ptr game, Stats::Ptr stats)
        : IObject(),
          game_(game),
          stats_(stats),
          zombieStats_(dynamic_cast<ZombieStats &>(*stats.get())),
          checkCollision_(ICollision::Ptr(new DynamicCollision))
    {
        rect_.pos = zombieStats_.position;
        rect_.size = SPRITE_SIZE;
        rect_.vel = {zombieStats_.speed / 2, 0};

        sprite_ = sg::GetEngine().CreateSprite(zombieStats_.path, zombieStats_.rgb);
    }

    Zombie::~Zombie() = default;

    void Zombie::Show(const Duration &duration) noexcept
    {
        PointType targetPoint = GetTarget();
        elapsedTime_ += duration;
        float seconds = duration.count() * 1e-9;
        if (elapsedTime_ > 70ms)
        {
            elapsedTime_ = 0s;
            count_++;
        }

        switch (state_)
        {
        case TO_PLAYER:
        {
            orientation_ = toOrientation(rect_.vel);
            if (targetPoint == NO_TARGET)
            {
                ::speedDown(rect_.vel, zombieStats_);
                zombieStats_.position = rect_.pos; // сохранить позицию
                state_ = SHAFFLE;
                rect_.vel /= 2;
            }
            else
            {
                rect_.vel = targetPoint - rect_.pos;
                ::speedUp(rect_.vel, zombieStats_);

                if ((targetPoint - rect_.pos).mag() < 50)
                {
                    rect_.vel = {0, 0};
                    state_ = ATTACK;
                }
            }

            break;
        }
        case SHAFFLE:
        {
            orientation_ = toOrientation(rect_.vel);

            if (targetPoint != NO_TARGET)
            {
                state_ = TO_PLAYER;
            }
            else
            {
                static const float SHUFFLE_DIST_ = alm::random::getReal(70, 150);
                if (rect_.pos.x > zombieStats_.position.x + SHUFFLE_DIST_)
                {
                    rect_.vel.x *= -1;
                }

                else if (rect_.pos.x < zombieStats_.position.x - SHUFFLE_DIST_)
                {
                    rect_.vel.x *= -1;
                }
            }

            break;
        }
        case ATTACK:
        {
            orientation_ = toOrientation((targetPoint - rect_.pos));
            rect_.vel = {0, 0};
            elapsedFromAttack_ += duration;

            // attack
            if (elapsedFromAttack_ > 500ms)
            {
                IdType playerId = game_->GetPlayerId();
                Attack(playerId);
                elapsedFromAttack_ = 0s;
                state_ = TO_PLAYER;
            }

            break;
        }
        default:
            break;
        }

        // collisions
        CollisionsExec(duration);
        rect_.pos += rect_.vel * seconds;
        DrawMe();
    }

    std::size_t Zombie::GetWidth() const noexcept
    {
        return SPRITE_SIZE.x;
    }

    std::size_t Zombie::GetHight() const noexcept
    {
        return SPRITE_SIZE.y;
    }

    CollisionType Zombie::GetCollisionType() const noexcept
    {
        return SOLID;
    }

    eObjectCategory Zombie::GetCategory() const noexcept
    {
        return ENEMY;
    }

    FRectPtr Zombie::GetRect() const noexcept
    {
        return &rect_;
    }

    void Zombie::OnAttack(Event::Ptr msg) noexcept
    {
    }

    IdType Zombie::GetId() const noexcept
    {
        return zombieStats_.id;
    }

    Stats::Ptr Zombie::GetStats() const noexcept
    {
        return stats_;
    }

    PointType Zombie::GetTarget()
    {
        auto playerPos = game_->GetPlayerPos();
        return (playerPos - rect_.pos).mag() < zombieStats_.dist ? playerPos : NO_TARGET;
    }

    void Zombie::DrawMe()
    {
        auto srcRect = getSrcRect(zombieStats_.type, orientation_, count_);
        auto destRect = getOffsetted(rect_, game_->GetOffset());
        sprite_->RenderCopy(srcRect, destRect);
    }

    void Zombie::CollisionsExec(const Duration &duration)
    {
        float elapsed = duration.count() * 1e-9;
        auto collideRects = game_->GetRects(ObjectsCategory(MAP | ENEMY));

        switch (state_)
        {
        case TO_PLAYER:
        {
            checkCollision_->Calculate(rect_, collideRects, elapsed);
            break;
        }
        case SHAFFLE:
        {
            if (checkCollision_->Calculate(rect_, collideRects, elapsed))
            {
                rect_.vel.x *= -1;
            }

            break;
        }
        case ATTACK:
        {
            break;
        }
        default:
            break;
        }
    }

    void Zombie::Attack(IdType id)
    {
        auto event = std::make_shared<AttackEvent>();
        event->setId(id);
        event->setDamage(zombieStats_.damage);
        eventer_(event);
    }

    void Zombie::OnInput(sg::MousePosType xy) noexcept
    {
    }
}