#pragma once

#include "ztypes.h"
#include "igame.h"
#include "imap.h"
#include "init_types.h"
#include "ztile.h"

#include <memory>
#include <fstream>

//!
//! \brief The ZMap class
//!
class ZWorld final : public zgame::BaseWorld
{

    using game_ptr_type = std::shared_ptr<zgame::IGame>;
    using rect_ptr_type = std::shared_ptr<zgame::rect>;
    using tile_ptr_type = std::unique_ptr<ZTile>;
    using stats_ptr_type = std::shared_ptr<zgame::WorldStats>;

public:

    //!
    //! \brief ZMap
    //! \param game
    //! \param res
    //! \param map
    //!
    ZWorld(game_ptr_type game, stats_ptr_type initData);

    //!
    ~ZWorld();

public:

    //!
    //! \brief showImpl
    //! \param i
    //!
    virtual void show(float fElapsedTime) noexcept override;

    //!
    //! \brief getRects
    //! \return
    //!
    virtual std::vector<rect_ptr_type> getRects() const noexcept override;

    //!
    //! \brief getWidthImpl
    //!
    virtual std::size_t getWidth() const noexcept override;

    //!
    //! \brief getHightImpl
    //!
    virtual std::size_t getHight() const noexcept override;

private:

    //!
    //! \brief getGraph
    //! \return
    //!
    math::Graph getGraph() noexcept;

    //!
    //! \brief isWayElement
    //! \param ch
    //! \return
    //!
    bool isWayElement(char ch);

    //!
    //! \brief getIndex
    //! \return
    //!
    int getIndex(uint32_t x, uint32_t y);

    //!
    //! \brief load
    //! \param path
    //!
    void load(std::fstream& file);

    //!
    //! \brief save
    //! \param path
    //!
    void save(std::fstream& file);

    //!
    //! \brief getTiles
    //! \param map
    //! \return
    //!
    std::vector<tile_ptr_type> getTiles(const std::vector<std::string>& map);

private:

    game_ptr_type game_;                      //!<
    std::string path_;                        //!<
    stats_ptr_type stats_;
    std::vector<tile_ptr_type> tiles_;        //!<
    std::vector<std::string> world_;          //!<
};
