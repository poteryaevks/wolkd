#pragma once

#include <memory>

#include "ztypes.h"
#include "init_types.h"
#include "igame.h"
#include "iobject.h"
#include "icollision.h"

namespace sg {
namespace engine {

template<class T> class ISprite;

}
}


//!
//! \brief The ZombieSprite class
//!
class ZZombie final : public zgame::BaseObject
{
    using GraphPtr = std::unique_ptr<math::Graph>;
    using rect_ptr_type = std::shared_ptr<zgame::rect>;
    using game_ptr_type = std::shared_ptr<zgame::IGame>;
    using stats_ptr_type = std::shared_ptr<zgame::ZombieStats>;
    using SpritePtr = std::unique_ptr<sg::engine::ISprite<ZZombie>>;

    enum eZombieState{

        TO_PLAYER,
        STAYING,
        SHAFFLE,
        ATTACK
    };

public:

    //!
    //! \brief ZombieObject
    //! \param game
    //! \param initializer
    //!
    ZZombie(game_ptr_type game, stats_ptr_type initData);

    //!
    ~ZZombie() = default;

public:

    //!
    //! \brief show
    //! \param i
    //!
    void show(float fElapsedTime) noexcept override;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getWidth() const noexcept override;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getHight() const noexcept override;

    //!
    //! \brief getCollisionType
    //! \return
    //!
    zgame::eObjectCollisionType getCollisionType() const noexcept override;

    //!
    //! \brief getCategory
    //! \return
    //!
    zgame::eObjectCategory getCategory() const noexcept override;

    //!
    //! \brief getRect
    //! \return
    //!
    rect_ptr_type getRect() const noexcept override;

    //!
    //! \brief attack
    //! \param damage
    //!
    virtual void getAttacked(std::shared_ptr<zgame::Event> msg) noexcept override;

    //!
    //! \brief getId
    //! \return
    //!
    virtual zgame::id_type getId() const noexcept override;

    //!
    //! \brief getStats
    //! \return
    //!
    virtual std::shared_ptr<zgame::Stats> getStats() const noexcept;

private:

    //!
    //! \brief getTarget
    //! \param inCollision
    //! \return
    //!
    zgame::point_type getTarget();

    //!
    //! \brief getPoint
    //! \param way
    //! \return
    //!
    zgame::point_type getPoint(std::list<zgame::point_type>& way);

    //!
    //! \brief pointToRect
    //! \param point
    //! \return
    //!
    zgame::rect pointToRect(const zgame::point_type& point) const noexcept;

    //!
    //! \brief rectToPoint
    //! \param rect
    //! \return
    //!
    zgame::point_type rectToPoint(const zgame::rect& rect);

    //!
    //! \brief drawMe
    //! \param state
    //! \param fElapsedTime
    //!
    void drawMe(float fElapsedTime);

    //!
    //! \brief collisionsExec
    //!
    void collisionsExec(float fElapsedTime);

    //!
    //! \brief doAttack
    //! \param id
    //!
    void doAttack(zgame::id_type id);

private:

    game_ptr_type game_;
    zgame::uint count_;
    float updateSprite_;
    rect_ptr_type rect_;
    stats_ptr_type stats_;
    GraphPtr graph_;
    zgame::eOrientation orientation_;
    eZombieState state_;
    SpritePtr sprite_;
    float FPS_;
    float attackFreq_;                //!< интервал между атаками
    float lastAttackTime_;

    const float SHUFFLE_DIST_;
};


