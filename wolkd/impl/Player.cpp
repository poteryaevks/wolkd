#include "Player.h"
#include "../helpers.hpp"

#include <iostream>
#include <math.h>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/engine/Engine.hpp>

namespace
{
    using namespace std::chrono_literals;
    const game::PointType SPRITE_SIZE{50, 50};
    const game::PointType IMG_SIZE{32, 32};
}

namespace game
{
    Player::Player(IGame::Ptr game, PlayerStats &&stats)
        : IObject(),
          game_(game),
          stats_(std::move(stats)),
          dynCollision_(sgraphics::ICollision::Create(sgraphics::CollisionType::Dynamic2)),
          staticCollision_(sgraphics::ICollision::Create(sgraphics::CollisionType::Static)),
          window_(sgraphics::GetEngine().GetWindow()),
          eventer_(sgraphics::GetEngine().GetEventer()),
          rect_({{(float)(window_->GetWidth() / 2), (float)(window_->GetHeight() / 2)}, SPRITE_SIZE}),
          sprite_(sgraphics::GetEngine().CreateSprite(stats_.path, stats_.rgb))
    {
        offset_.pos = stats_.position;
        offset_.size = SPRITE_SIZE;
    }

    Player::~Player() = default;

    const FRectType &Player::GetRect() const noexcept
    {
        return rect_;
    }

    void Player::Show(const Duration &duration) noexcept
    {
        if (eventer_->LeftMouseDown())
        {
            mousePosition_ = eventer_->MousePosition();
        }

        rect_.vel = mousePosition_ - rect_.pos;
        rect_.vel = rect_.vel.norm() * stats_.speed;

        auto renderer = sgraphics::GetEngine().GetRenderer();
        float seconds = duration.count() * 1e-9;
        auto tiles = game_->GetRects(ObjectsCategory(MAP));
        dynCollision_->Calculate(rect_, ::Convert<FRectRefs, sgraphics::ICollision::RectsType>(tiles), seconds);
        offset_.pos += rect_.vel * seconds;
        mousePosition_ -= rect_.vel * seconds;
#ifdef NDEBUG_
        renderer->DrawRect(FRectType({mousePosition_, SPRITE_SIZE}), {160, 160, 0, 255});
        for (const auto &rect : rect_.contact)
        {
            if (rect)
            {
                renderer->DrawRect(*rect, {255, 160, 0, 255});
            }
        }

        renderer->DrawRect(rect_, {255, 100, 0, 255});
#endif
        if (areEqual(mousePosition_, rect_.pos, 1))
        {
            rect_.vel = {0, 0};
        }

        DrawMe(duration);
    }

    CollisionType Player::GetCollisionType() const noexcept
    {
        return SOLID;
    }

    eObjectCategory Player::GetCategory() const noexcept
    {
        return PLAYER;
    }

    const std::size_t &Player::GetWidth() const noexcept
    {
        return SPRITE_SIZE.x;
    }

    const std::size_t &Player::GetHight() const noexcept
    {
        return SPRITE_SIZE.y;
    }

    void Player::OnEvent(Event::Ptr event) noexcept
    {
        if (!event)
            return;

        switch (event->getType())
        {
        case EventType::ATTACK:
        {
            auto attackEvent = dynamic_cast<AttackEvent *>(event.get());
            if (attackEvent)
            {
                stats_.hp -= attackEvent->getDamage();
                stats_.hp = stats_.hp < 0 ? 0 : stats_.hp;
            }
            break;
        }
        default:
            break;
        }
    }

    IdType Player::GetId() const noexcept
    {
        return stats_.id;
    }

    const Stats &Player::GetStats() const noexcept
    {
        return stats_;
    }

    void Player::DrawMe(const Duration &duration)
    {
        static Duration UpdateDur{};
        static std::uint32_t Count{};
        static eOrientation orientation{};

        orientation = (rect_.vel.x == 0 && rect_.vel.y == 0) ? orientation : toOrientation(rect_.vel);
        auto update = [&](const Duration &duration, Duration period)
        {
            UpdateDur += duration;
            if (period < UpdateDur)
            {
                UpdateDur = {};
                Count++;
            }
        };

        if (rect_.vel.x != 0 || rect_.vel.y != 0)
        {
            update(duration, 50ms);
        }

        FRectType srcRect{{(float)(Count % 3) * IMG_SIZE.x, orientation * IMG_SIZE.y}, IMG_SIZE};
        sprite_->RenderCopy(srcRect, rect_);
    }
} // namespace game
