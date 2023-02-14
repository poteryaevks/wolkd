#pragma once

#include "Types.h"
#include "IGame.h"
#include "IWorld.h"

#include "Stats.h"
#include "Tiles.h"

#include <memory>
#include <iostream>

namespace game
{
    class WorldImpl final : public IWorld
    {
    public:
        WorldImpl(IGame::Ptr game, Stats::Ptr initData);
        ~WorldImpl();

        void Show(const Duration &) noexcept override;
        std::vector<FRectPtr> GetRects() const noexcept override;
        std::size_t GetWidth() const noexcept override;
        std::size_t GetHight() const noexcept override;

    private:
        bool isWayElement(char ch);
        int getIndex(std::uint32_t x, std::uint32_t y);
        void load(std::istream &);
        void save(std::ostream &);
        std::vector<ITile::Ptr> getTiles(const std::vector<std::string> &map);

    private:
        IGame::Ptr game_;
        Stats::Ptr stats_;
        WorldStats* m_worldStats;
        std::vector<ITile::Ptr> tiles_;
        std::vector<std::string> world_;
    };
}
