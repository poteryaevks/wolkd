#include "Player.h"
#include "CollisionImpl.h"

#include <iostream>
#include <math.h>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/engine/Engine.hpp>

namespace
{
    using namespace std::chrono_literals;
    const game::PointType SPRITE_SIZE{50, 50};
}

namespace game
{
    Player::Player(IGame::Ptr game, Stats::Ptr initData)
        : IObject(),
          game_(game),
          stats_(initData),
          checkCollision_(ICollision::Ptr(new DynamicCollision))
    {
        FPS_ = stats_->speed / 10;
        auto window = sg::GetEngine().GetWindow();
        sprite_ = sg::GetEngine().CreateSprite(stats_->path, stats_->rgb);

        realRect_.pos = stats_->position;
        realRect_.size = SPRITE_SIZE;

        screenRect_.pos = {(float)(window->GetWidth() / 2), (float)(window->GetHeight() / 2)};
        screenRect_.size = SPRITE_SIZE;
    }

    Player::~Player() = default;

    FRectPtr Player::GetRect() const noexcept
    {
        return &realRect_;
    }

    void Player::Show(const Duration &duration) noexcept
    {
        float seconds = duration.count() * 1e-9;

        auto collideRects = game_->GetRects(ObjectsCategory(game::MAP | game::ENEMY));

        checkCollision_->Calculate(realRect_, collideRects, seconds);

        realRect_.pos += realRect_.vel * seconds;
        mouseOffset_ -= realRect_.vel * seconds;

#ifdef NDEBUG
        auto renderer = sg::GetEngine().GetRenderer();
        renderer->DrawRect(FRectType({{mouseOffset_.x, mouseOffset_.y}, {SPRITE_SIZE.x, SPRITE_SIZE.y}}),
                           {160, 160, 0, 255});
#endif

        if (areEqual(mouseOffset_, screenRect_.pos, 1))
        {
            realRect_.vel = {0, 0};
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

    std::size_t Player::GetWidth() const noexcept
    {
        return SPRITE_SIZE.x;
    }

    std::size_t Player::GetHight() const noexcept
    {
        return SPRITE_SIZE.y;
    }

    void Player::OnAttack(Event::Ptr event) noexcept
    {
        AttackEvent *attack = dynamic_cast<AttackEvent *>(event.get());
        if (attack)
        {
            if (stats_->hp > 0)
                stats_->hp -= attack->getDamage();

            if (stats_->hp < 0)
                stats_->hp = 0;
        }
    }

    IdType Player::GetId() const noexcept
    {
        return stats_->id;
    }

    Stats::Ptr Player::GetStats() const noexcept
    {
        return stats_;
    }

    void Player::DrawMe(const Duration &duration)
    {
        State state;

        if (realRect_.vel.x == 0 && realRect_.vel.y == 0)
        {
            updateSprite_ = 0s;
            state = STAYING;
        }
        else
        {
            state = MOVING;
        }

        orientation_ = toOrientation(realRect_.vel);

        switch (state)
        {
        case MOVING:
        {
            updateSprite_ += duration;
            if (updateSprite_ > 50ms)
            {
                updateSprite_ = 0s;
                count_++;
            }

            break;
        }
        case STAYING:
        {
            break;
        }
        default:
            break;
        }

        FRectType srcRect{{(float)(count_ % 3) * 32, orientation_ * 32}, {32, 32}};
        sprite_->RenderCopy(srcRect, screenRect_);
    }

    void Player::OnInput(sg::MousePosType xy) noexcept
    {
        mouseOffset_ = {(float)xy.first, (float)xy.second};
        realRect_.vel = mouseOffset_ - screenRect_.pos;
        realRect_.vel = realRect_.vel.norm() * stats_->speed;
    }
} // namespace zgame
