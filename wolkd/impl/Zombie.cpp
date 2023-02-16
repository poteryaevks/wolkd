#include "Zombie.h"
#include "../Types.h"
#include "../helpers.hpp"

#include <thread>
#include <iostream>
#include <math.h>

#include <alm/misc/random.h>
#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/Engine.hpp>

namespace
{
    using namespace game;
    using namespace std::chrono_literals;

    const PointType SPRITE_SIZE{50, 50};
    constexpr int PNG_SIZE{32};

    FRectType GetSrcRect(ZombieStats::eZombieType type, eOrientation orient, std::uint32_t count)
    {
        std::uint32_t xIndex{type - (type / 4) * 4};
        std::uint32_t yIndex{type / 4};

        return FRectType({{(((std::int32_t)count % 3) * PNG_SIZE) + PNG_SIZE * 3 * xIndex, (orient * PNG_SIZE) + PNG_SIZE * 4 * yIndex},
                          {PNG_SIZE, PNG_SIZE}});
    }

    template <class T>
    void SpeedUp(olc::v2d_generic<T> &vel, const ZombieStats &data)
    {
        vel = vel.norm() * data.speed;
    }

    template <class T>
    void SpeedDown(olc::v2d_generic<T> &vel, const ZombieStats &data)
    {
        vel = {data.speed, 0};
    }

    FRectType GetOffsetted(const FRectType &rect, FVectType offset)
    {
        FRectType offseted(rect);
        offseted.pos += offset;
        return offseted;
    }
}

namespace game
{
    Zombie::Zombie(IGame::Ptr game, ZombieStats &&stats)
        : IObject(),
          game_(game),
          renderer_(sg::GetEngine().GetRenderer()),
          stats_(std::move(stats)),
          dynCollision_(sg::ICollision::Create(sg::CollisionType::Dynamic2)),
          staticCollision_(sg::ICollision::Create(sg::CollisionType::Static))
    {
        realRect_.pos = stats_.position;
        realRect_.size = SPRITE_SIZE;
        realRect_.vel = {stats_.speed / 2, 0};
        sprite_ = sg::GetEngine().CreateSprite(stats_.path, stats_.rgb);
    }

    Zombie::~Zombie() = default;

    void Zombie::Show(const Duration &duration) noexcept
    {
        rect_ = ::GetOffsetted(realRect_, game_->GetOffset());
        auto player = game_->GetPlayerRect();
        auto state = UpdateState(player);
        float seconds = duration.count() * 1e-9;

        switch (state)
        {
        case State::TO_PLAYER:
        {
            rect_.vel = player.pos - rect_.pos;
            if ((player.pos - rect_.pos).mag() < 50)
            {
                Attack(duration);
            }
            break;
        }
        case State::SHUFFLE:
        {
            Shuffle(duration);
            break;
        }
        default:
            break;
        }

        Collide(duration, state);
        realRect_.vel = rect_.vel;
        realRect_.pos += realRect_.vel * seconds;
        DrawMe(duration);
        prevState_ = state;
    }

    void Zombie::Shuffle(const Duration &duration)
    {
        static Duration UpdateDur{};
        UpdateDur += duration;

        if (prevState_ == State::TO_PLAYER)
        {
            rect_.vel = {stats_.speed / 2, 0};
        }

        
        if (UpdateDur > 5s)
        {
            rect_.vel.x = std::max(rect_.vel.x, stats_.speed / 2);
            rect_.vel.x *= -1;
            UpdateDur = 0s;
        }
    }

    const std::size_t &Zombie::GetWidth() const noexcept
    {
        return SPRITE_SIZE.x;
    }

    const std::size_t &Zombie::GetHight() const noexcept
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

    const FRectType &Zombie::GetRect() const noexcept
    {
        return rect_;
    }

    void Zombie::OnEvent(Event::Ptr msg) noexcept
    {
    }

    IdType Zombie::GetId() const noexcept
    {
        return stats_.id;
    }

    const Stats &Zombie::GetStats() const noexcept
    {
        return stats_;
    }

    Zombie::State Zombie::UpdateState(const FRectType &player)
    {
#ifdef NDEBUG_
        renderer_->DrawLine(player.pos, rect_.pos, {160, 160, 0, 255});
#endif
        return ((player.pos - rect_.pos).mag() < stats_.dist) ? State::TO_PLAYER : State::SHUFFLE;
    }

    void Zombie::DrawMe(const Duration &duration)
    {
        static Duration UpdateDur{};
        static std::uint32_t Count{};
        static eOrientation orientation{};

        orientation = toOrientation(rect_.vel);
        auto update = [&](const Duration &duration, Duration period)
        {
            UpdateDur += duration;
            if (period < UpdateDur)
            {
                UpdateDur = {};
                Count++;
            }
        };

        update(duration, 100ms);
        auto srcRect = ::GetSrcRect(stats_.type, orientation, Count);
        sprite_->RenderCopy(srcRect, rect_);
    }

    void Zombie::Collide(const Duration &duration, State state)
    {
        float elapsed = duration.count() * 1e-9;
        auto tiles = game_->GetRects(ObjectsCategory(MAP));
        bool collision = dynCollision_->Calculate(rect_, ::Convert<FRectRefs, sg::ICollision::RectsType>(tiles), elapsed);
#ifdef NDEBUG_
        for (const auto &tile : tiles)
        {
            renderer_->DrawRect(tile, {160, 160, 0, 255});
        }

        renderer_->DrawRect(rect_, {255, 100, 0, 255});
#endif
        if (collision && state == State::SHUFFLE)
        {
            rect_.vel.x *= -1;
        }
    }

    void Zombie::Attack(const Duration &duration)
    {
        static Duration ElapsedFromAttack{};

        rect_.vel = {0, 0};
        ElapsedFromAttack += duration;

        // attack
        if (ElapsedFromAttack > 300ms)
        {
            auto player = game_->GetPlayerRect();
            renderer_->DrawRect(player, {255, 0, 0, 255}, true);
            std::this_thread::sleep_for(100ms);
            auto id = game_->GetPlayerId();
            auto event = std::make_shared<AttackEvent>();
            event->SetId(id);
            event->setDamage(stats_.damage);
            eventer_(event);
            ElapsedFromAttack = 0s;
        }
    }

    void Zombie::OnInput(sg::MousePosType xy) noexcept
    {
    }
}
