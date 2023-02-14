#include "WorldImpl.h"

#include <sgraphics/geometry/olc.h>
#include <fstream>

namespace
{
    const game::PointType TILE_SIZE{60, 60};
}

namespace game
{

    WorldImpl::WorldImpl(IGame::Ptr game, WorldStats &&stats)
        : IWorld(),
          game_(game),
          stats_(std::move(stats))
    {
        std::ifstream file(stats_.map, std::ios::binary);
        if (!file)
        {
        }

        load(file);
    }

    WorldImpl::~WorldImpl()
    {
        std::ofstream file(stats_.map, std::ios::binary);
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

    const FRectRefs &WorldImpl::GetRects() const noexcept
    {
        return rects_;
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
                    TILE_FACTORY.Create(ch, FRectType({{x * TILE_SIZE.x, y * TILE_SIZE.y}, TILE_SIZE}), stats_.tiles, stats_.rgb));

                if (tiles.back()->getType() == SOLID)
                {
                    rects_.emplace_back(tiles.back()->GetRect());
                }
            }
        }

        return tiles;
    }
}