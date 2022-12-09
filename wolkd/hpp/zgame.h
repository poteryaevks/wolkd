#pragma once

#include "igame.h"
#include "imap.h"
#include "iobject.h"


//!
//! \brief The WolkD class
//!
class WolkingZombies : public zgame::IGame,
       public std::enable_shared_from_this<WolkingZombies>
{
    using enemy_ptr_type = std::unique_ptr<zgame::BaseObject>;
    using player_ptr_type = std::unique_ptr<zgame::BaseObject>;
    using map_ptr_type = std::unique_ptr<zgame::BaseWorld>;

public:

    //!
    WolkingZombies() = default;

    //!
    ~WolkingZombies() = default;

    //!
    //! \brief init
    //!
    void initImpl();

    //!
    //! \brief getEvents
    //! \param event
    //!
    void updateImpl(float tp);

    //!
    //! \brief quit
    //!
    void quitImpl();

    //!
    //! \brief getPlayerPos
    //! \return
    //!
    virtual zgame::point_type getPlayerPos() const noexcept override;

    //!
    //! \brief getGraph
    //! \return
    //!
    virtual math::Graph getGraph() noexcept override;

    //!
    //! \brief getOffset
    //!
    virtual zgame::vect_type getOffset() const noexcept override;

    //!
    //! \brief getRects
    //! \return
    //!
    std::vector<std::shared_ptr<zgame::rect>> getRects(ObjectsCategory category) const noexcept override;

    //!
    //! \brief getPlayerId
    //! \return
    //!
    virtual zgame::id_type getPlayerId() const noexcept override;

    //!
    //! \brief addEvent
    //! \param msg
    //!
    void addEvent(std::shared_ptr<zgame::Event> msg) noexcept;

private:

    void printStats();

private:

    std::list<enemy_ptr_type> enemies;
    map_ptr_type world_;
    player_ptr_type player_;
    zgame::point_type defPosition_;
};
