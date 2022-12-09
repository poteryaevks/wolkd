#pragma once

#include <memory>

#include "ztypes.h"
#include "init_types.h"
#include "iinit.h"
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
class ZPlayer final : public zgame::BaseObject
{
    friend class sg::engine::ISprite<ZPlayer>;

    using game_ptr_type = std::shared_ptr<zgame::IGame>;
    using rect_ptr_type = std::shared_ptr<zgame::rect>;
    using stats_ptr_type = std::shared_ptr<zgame::PlayerStats>;
    using InitPtr = std::shared_ptr<zgame::IInit>;
    using SpritePtr = std::unique_ptr<sg::engine::ISprite<ZPlayer>>;

    enum eSpriteState{

        MOVING,
        STAYING
    };

public:

    //!
    //! \brief PlayerObject
    //! \param game
    //! \param initData
    //!
    ZPlayer(game_ptr_type game, stats_ptr_type initData);

    ~ZPlayer() = default;

public:

    //!
    //! \brief getRect
    //! \return
    //!
    virtual rect_ptr_type getRect() const noexcept;

    //!
    //! \brief show
    //! \param i
    //!
    void show(float fElapsedTime) noexcept override;

    //!
    //! \brief getCollisionType
    //! \return
    //!
    virtual zgame::eObjectCollisionType getCollisionType() const noexcept override;

    //!
    //! \brief Category
    //! \return
    //!
    virtual zgame::eObjectCategory getCategory() const noexcept override;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getWidth() const noexcept override;

    //!
    //! \brief getWidth
    //!
    virtual std::size_t getHight() const noexcept override;

    //!
    //! \brief attack
    //! \param damage
    //!
    virtual void getAttacked(std::shared_ptr<zgame::Event> msg) noexcept override;

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
    //! \brief userInput
    //!
    void userInput();

    //!
    //! \brief drawMe
    //! \param fElapsedTime
    //!
    void drawMe(float fElapsedTime);

    //!
    //! \brief drawStats
    //!
    void drawStats();

private:

    game_ptr_type game_;
    stats_ptr_type stats_;
    Uint32 count_;
    float updateSprite_;

    rect_ptr_type realRect_;
    rect_ptr_type screenRect_;
    zgame::point_type mouseOffset_;
    zgame::eOrientation orientation_;

    SpritePtr sprite_;
    float FPS_;
};


