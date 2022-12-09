#include "zombie_object.h"

#include <iostream>
#include <math.h>

#include <bfs/pathfinder.h>

#include <almaz/misc/random.h>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/polygon_funcs.h>
#include <sgraphics/engine/isprite.h>

namespace  {

const zgame::point_type NO_TARGET({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()});
const zgame::point_type SPRITE_SIZE { 50, 50 };

constexpr int PNG_SIZE{32};

zgame::rect getSrcRect(zgame::ZombieStats::eZombieType type, zgame::eOrientation orient, zgame::uint count)
{
    zgame::uint xIndex{ type - (type / 4) * 4 };
    zgame::uint yIndex{ type / 4 };

    return zgame::rect({
                           { (((int32_t)count % 3) * PNG_SIZE) + PNG_SIZE * 3 * xIndex, (orient * PNG_SIZE) + PNG_SIZE * 4 * yIndex},
                           { PNG_SIZE, PNG_SIZE }
                       });
}

template <class T>
void speedUp(olc::v2d_generic<T>& vel, const zgame::ZombieStats& data)
{
    switch (data.anger) {
    case zgame::ZombieStats::eAnger::neutral:
        vel = vel.norm() * data.speed * 1;
        break;
    case zgame::ZombieStats::eAnger::angry:
        vel = vel.norm() * data.speed * 1.5;
        break;
    case zgame::ZombieStats::eAnger::very_angry:
        vel = vel.norm() * data.speed * 2;
        break;
    default:
        break;
    }
}

template <class T>
void speedDown(olc::v2d_generic<T>& vel, const zgame::ZombieStats& data)
{
    vel = { data.speed, 0};
}

zgame::rect getOffsetted(const zgame::rect& rect, zgame::vect_type offset)
{
    zgame::rect offseted(rect);
    offseted.pos += offset;
    return offseted;
}

}


ZZombie::ZZombie(game_ptr_type game, stats_ptr_type initData)
    : zgame::BaseObject(),
      game_(game),
      count_(0),
      updateSprite_(0),
      orientation_(zgame::RIGHT),
      state_(SHAFFLE),
      SHUFFLE_DIST_(almaz::random::getReal(70, 150)),
      lastAttackTime_(std::numeric_limits<float>::max()),
      stats_(initData)
{
    rect_      = std::make_shared<zgame::rect>(
                zgame::rect({ stats_->position, SPRITE_SIZE })
                );

    FPS_        = stats_->speed / 10;
    sprite_     = std::make_unique<sg::engine::ISprite<ZZombie>>(stats_->path, stats_->rgb);
    rect_->size = SPRITE_SIZE;
    rect_->vel  = { stats_->speed, 0 };

    switch (stats_->anger) {
    case zgame::ZombieStats::eAnger::neutral:
        attackFreq_ = 2;
        break;
    case zgame::ZombieStats::eAnger::angry:
        attackFreq_ = 1;
        break;
    case zgame::ZombieStats::eAnger::very_angry:
        attackFreq_ = 0.5;
        break;
    default:
        break;
    }

#ifdef BFS
    graph_ = game_->getGraph();
#endif
}


void ZZombie::show(float fElapsedTime) noexcept
{
    zgame::point_type targetPoint = getTarget();

    updateSprite_ += fElapsedTime;

    switch (state_) {
    case TO_PLAYER:
    {
        orientation_ = zgame::toOrientation(rect_->vel);

        if(updateSprite_ > 1 / FPS_){

            updateSprite_ = 0;
            count_++;
        }

        if(targetPoint == NO_TARGET){

            speedDown(rect_->vel, *stats_);
            stats_->position = rect_->pos; // сохранить позицию
            state_ = SHAFFLE;
        }
        else{

            rect_->vel = targetPoint - rect_->pos;
            speedUp(rect_->vel, *stats_);
        }

        break;
    }
    case SHAFFLE:
    {
        orientation_ = zgame::toOrientation(rect_->vel);

        if(updateSprite_ > 1 / FPS_){

            updateSprite_ = 0;
            count_++;
        }

        if(targetPoint != NO_TARGET){

            state_ = TO_PLAYER;
        }
        else{

            if      (rect_->pos.x > stats_->position.x + SHUFFLE_DIST_ ) rect_->vel.x *= -1;
            else if (rect_->pos.x < stats_->position.x - SHUFFLE_DIST_ ) rect_->vel.x *= -1;
        }

        break;
    }
    case ATTACK:
    {
        orientation_ = zgame::toOrientation((targetPoint - rect_->pos));
        rect_->vel = {0,0};

        // attack
        if(lastAttackTime_ > attackFreq_){

            zgame::id_type playerId = game_->getPlayerId();
            doAttack(playerId);
            lastAttackTime_ = 0;
            state_ = TO_PLAYER;
        }
        else
            lastAttackTime_ += fElapsedTime;

        break;
    }
    default:
        break;
    }

    // collisions
    collisionsExec(fElapsedTime);

    rect_->pos += rect_->vel * fElapsedTime;
    drawMe(fElapsedTime);
}


std::size_t ZZombie::getWidth() const noexcept
{
    return SPRITE_SIZE.x;
}

std::size_t ZZombie::getHight() const noexcept
{
    return SPRITE_SIZE.y;
}


zgame::eObjectCollisionType ZZombie::getCollisionType() const noexcept
{
    return zgame::SOLID;
}


zgame::eObjectCategory ZZombie::getCategory() const noexcept
{
    return zgame::ENEMY;
}


ZZombie::rect_ptr_type ZZombie::getRect() const noexcept
{
    return rect_;
}

void ZZombie::getAttacked(std::shared_ptr<zgame::Event> msg) noexcept
{

}

zgame::id_type ZZombie::getId() const noexcept
{
    return stats_->id;
}

std::shared_ptr<zgame::Stats> ZZombie::getStats() const noexcept
{
    return stats_;
}


zgame::point_type ZZombie::getTarget()
{
    zgame::point_type targetPoint;

#ifdef BFS

    if(inCollision){

        if(way_.empty()){

            bfs_ = true;

            auto playerPos = game_->getPlayerPos();
            zgame::point_type ownPoint = rectToPoint(rect_);
            zgame::point_type playerPoint({playerPos.x / rect_->size.x, playerPos.y / rect_->size.y});

            way_ = graph_.findWay(
                        ownPoint,
                        playerPoint
                        );
        }

        targetPoint = getPoint(way_) + game_->getOffset();
    }
    else{

        if(bfs_){

            targetPoint = getPoint(way_) + game_->getOffset();

            if(zgame::areEqual(targetPoint.x, getRect().pos.x, 0.1)
                    && zgame::areEqual(targetPoint.y, getRect().pos.y, 0.1)){

                way_.pop_back();
                targetPoint = getPoint(way_) + game_->getOffset();
            }

            if(way_.empty())
                bfs_ = false;
        }
        else{

            targetPoint = game_->getPlayerPos() + game_->getOffset();
        }
    }

#else
    auto playerPos = game_->getPlayerPos();

    switch (stats_->anger) {
    case zgame::ZombieStats::eAnger::neutral:
        return (playerPos - rect_->pos).mag() < 100 ? playerPos : NO_TARGET;
        break;

    case zgame::ZombieStats::eAnger::angry:
        return (playerPos - rect_->pos).mag() < 200 ? playerPos : NO_TARGET;
        break;

    case zgame::ZombieStats::eAnger::very_angry:
        return (playerPos - rect_->pos).mag() < 300 ? playerPos : NO_TARGET;
        break;

    default:
        break;
    }

#endif

    return targetPoint;
}


zgame::point_type ZZombie::getPoint(std::list<zgame::point_type>& way)
{
    if(way.empty()) return rect_->pos;

    zgame::point_type targetPoint = way.back();
    return { targetPoint.x * SPRITE_SIZE.x, targetPoint.y * SPRITE_SIZE.y };
}


zgame::rect ZZombie::pointToRect(const zgame::point_type& point) const noexcept
{
    return { { point.x * SPRITE_SIZE.x, point.y * SPRITE_SIZE.y }, SPRITE_SIZE };
}


zgame::point_type ZZombie::rectToPoint(const zgame::rect& rect)
{

    return { (rect.pos.x / rect.size.x), (rect.pos.y / rect.size.y) };
}


void ZZombie::drawMe(float /*fElapsedTime*/)
{
    auto srcRect = getSrcRect(stats_->type, orientation_, count_);
    auto destRect = getOffsetted(*rect_, game_->getOffset());
    sprite_->renderCopy(srcRect, destRect);
}

void ZZombie::collisionsExec(float fElapsedTime)
{
    ICollision<DynamicCollision> checkCollision(fElapsedTime);
    auto collideRects = game_->getRects(ObjectsCategory(zgame::MAP | zgame::ENEMY));
    auto collidePlayer = game_->getRects(zgame::PLAYER);

    switch (state_){
    case TO_PLAYER:

        if(checkCollision(*rect_, collidePlayer)){

            state_ = ATTACK;
        }
        else{

            checkCollision(*rect_, collideRects);
        }

        break;
    case SHAFFLE:
    {
        if(checkCollision(*rect_, collideRects)){
            rect_->vel.x *= -1;
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

void ZZombie::doAttack(zgame::id_type id)
{
    std::shared_ptr<zgame::AttackEvent> msg = std::make_shared<zgame::AttackEvent>();
    msg->setId(id);

    switch (stats_->anger) {

    case zgame::ZombieStats::eAnger::neutral:
        msg->setDamage(stats_->damage * 1);
        break;
    case zgame::ZombieStats::eAnger::angry:
        msg->setDamage(stats_->damage * 2);
        break;
    case zgame::ZombieStats::eAnger::very_angry:
        msg->setDamage(stats_->damage * 3);
        break;
    default:
        break;
    }

    eventer_(msg);
}




