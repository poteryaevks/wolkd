#include "player_object.h"

#include <iostream>
#include <math.h>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/isprite.h>
#include <sgraphics/engine/polygon_funcs.h>


namespace  {

const zgame::point_type SPRITE_SIZE { 50, 50 };

}

ZPlayer::ZPlayer(game_ptr_type game, stats_ptr_type initData)
    : BaseObject(),
      game_(game),
      stats_(initData),
      count_(0),
      updateSprite_(0),
      orientation_(zgame::RIGHT)
{
    FPS_ = stats_->speed / 10;
    sprite_ = std::make_unique<sg::engine::ISprite<ZPlayer>>(stats_->path, stats_->rgb);

    realRect_   = std::make_shared<zgame::rect>(
                zgame::rect({ stats_->position, SPRITE_SIZE })
                );

    screenRect_ = std::make_shared<zgame::rect>(
                zgame::rect({ { (float)(sg::engine::getWidth() / 2), (float)(sg::engine::getHeight() / 2) }, SPRITE_SIZE })
                );
}


ZPlayer::rect_ptr_type ZPlayer::getRect() const noexcept
{
    return realRect_;
}


void ZPlayer::show(float fElapsedTime) noexcept {

    userInput();

    ICollision<DynamicCollision> dynCheckCollision(fElapsedTime);

    auto collideRects = game_->getRects(ObjectsCategory(zgame::MAP | zgame::ENEMY));

    dynCheckCollision(*realRect_, collideRects);

    realRect_->pos += realRect_->vel * fElapsedTime;
    mouseOffset_   -= realRect_->vel * fElapsedTime;

#ifdef NDEBUG
    sg::engine::drawRect(
                zgame::rect({{ mouseOffset_.x, mouseOffset_.y },{ SPRITE_SIZE.x, SPRITE_SIZE.y }}),
    { 160, 160, 0, 255 }
                );
#endif

    if(zgame::areEqual(mouseOffset_, screenRect_->pos, 1)){

        realRect_->vel = { 0, 0 };
    }

    drawMe(fElapsedTime);
}


zgame::eObjectCollisionType ZPlayer::getCollisionType() const noexcept
{
    return zgame::SOLID;
}


zgame::eObjectCategory ZPlayer::getCategory() const noexcept
{
    return zgame::PLAYER;
}


std::size_t ZPlayer::getWidth() const noexcept
{
    return SPRITE_SIZE.x;
}


std::size_t ZPlayer::getHight() const noexcept
{
    return SPRITE_SIZE.y;
}

void ZPlayer::getAttacked(std::shared_ptr<zgame::Event> msg) noexcept
{
    zgame::AttackEvent* attack = dynamic_cast<zgame::AttackEvent*>(msg.get());
    if(attack){
        if(stats_->hp > 0)
            stats_->hp -= attack->getDamage();

        if(stats_->hp < 0)
            stats_->hp = 0;
    }
}

zgame::id_type ZPlayer::getId() const noexcept
{
    return stats_->id;
}

std::shared_ptr<zgame::Stats> ZPlayer::getStats() const noexcept
{
    return stats_;
}


void ZPlayer::userInput()
{
    const SDL_Event& event = sg::engine::getEvent();

    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        auto xy = sg::engine::getMousePosition();

        mouseOffset_ = { (float)xy.first, (float)xy.second };

        realRect_->vel = mouseOffset_ - screenRect_->pos;
        realRect_->vel = realRect_->vel.norm() * stats_->speed;

        break;
    }
    default:
        break;
    }
}


void ZPlayer::drawMe(float fElapsedTime)
{
    eSpriteState state;

    if(realRect_->vel.x == 0 && realRect_->vel.y == 0){

        updateSprite_ = 0;
        state = STAYING;
    }
    else{
        state = MOVING;
    }

    orientation_ = zgame::toOrientation(realRect_->vel);

    switch (state)
    {
    case MOVING:
    {
        updateSprite_ += fElapsedTime;

        if(updateSprite_ > 1 / FPS_){

            updateSprite_ = 0;
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

    zgame::rect  srcRect {
        { (count_ % 3) * 32, orientation_ * 32 },
        {32, 32}
    };

    // draw zombie in the middle of screen
    sprite_->renderCopy(srcRect, *screenRect_);
}

