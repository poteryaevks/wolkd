#include "WorldImpl.h"

#include <sgraphics/geometry/olc.h>
#include <fstream>

namespace
{
    constexpr std::uint8_t TILE_SIZE{60};
}

namespace game
{

    WorldImpl::WorldImpl(IGame::Ptr game, Stats::Ptr stats)
        : IWorld(),
          game_(game),
          stats_(stats),
          m_worldStats(dynamic_cast<WorldStats *>(stats.get()))
    {
        std::ifstream file(m_worldStats->map, std::ios::binary);
        if (!file)
        {
        }

        load(file);
    }

    WorldImpl::~WorldImpl()
    {
        std::ofstream file(m_worldStats->map, std::ios::binary);
        if (file)
        {
            save(file);
        }
    }

    void WorldImpl::Show(const Duration &duration) noexcept
    {
        for (auto &tile : tiles_)
        {
            tile->show(game_->GetOffset(), duration);
        }
    }

    std::vector<FRectPtr> WorldImpl::GetRects() const noexcept
    {
        std::vector<FRectPtr> rects;

        for (const auto &tile : tiles_)
        {
            if (tile->getType() == SOLID)
            {
                rects.emplace_back(tile->GetRect());
            }
        }

        return std::move(rects);
    }

    bool WorldImpl::isWayElement(char ch)
    {
        return (ch == '-' || ch == '.');
    }

    int WorldImpl::getIndex(uint32_t x, uint32_t y)
    {
        return (y * world_[0].size() + x);
    }

    std::size_t WorldImpl::GetWidth() const noexcept
    {
        return world_[0].size();
    }

    std::size_t WorldImpl::GetHight() const noexcept
    {
        return world_.size();
    }

    void WorldImpl::load(std::istream &stream)
    {
        std::string line;
        while (stream >> line)
        {
            world_.emplace_back(line);
        }

        tiles_ = getTiles(world_);
    }

    void WorldImpl::save(std::ostream &stream)
    {
        for (const auto &line : world_)
        {
            stream << line << '\n';
        }
    }

    std::vector<ITile::Ptr> WorldImpl::getTiles(const std::vector<std::string> &map)
    {
        std::vector<ITile::Ptr> tiles;
        tiles.reserve(2000);

        // ..init rects..
        for (std::size_t y = 0; y < map.size(); ++y)
        {
            for (std::size_t x = 0; x < map[y].size(); ++x)
            {
                auto ch = map[y][x];
                tiles.emplace_back(
                    TILE_FACTORY.Create(
                        ch,
                        FRectType({{x * TILE_SIZE, y * TILE_SIZE}, {TILE_SIZE, TILE_SIZE}}),
                        m_worldStats->tiles,
                        m_worldStats->rgb));
            }
        }

        return tiles;
    }
}