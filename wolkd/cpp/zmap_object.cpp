#include "zmap_object.h"

#include <bfs/pathfinder.h>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/polygon_funcs.h>

namespace  {

constexpr uint8_t TILE_SIZE {60};

}


ZWorld::ZWorld(game_ptr_type game, stats_ptr_type initData)
    : zgame::BaseWorld(),
      game_(game),
      stats_(initData)
{
    std::fstream file;
    file.exceptions (std::ifstream::badbit);
    file.open(stats_->map, file.binary | file.in);
    load(file);
}

ZWorld::~ZWorld()
{
    std::fstream file;
    file.open(stats_->map, file.binary | file.out);
    if(file.is_open())
        save(file);
}

void ZWorld::show(float fElapsedTime) noexcept
{
    for(auto& tile : tiles_){

        tile->show(game_->getOffset(), fElapsedTime);
    }
}

std::vector<ZWorld::rect_ptr_type> ZWorld::getRects() const noexcept
{
    std::vector<std::shared_ptr<zgame::rect>> rects;

    for(const auto& tile : tiles_){

        if(tile->getType() == zgame::SOLID){

            rects.emplace_back(
                        tile->getRect()
                        );
        }
    }

    return std::move(rects);
}

//!
//! \brief getGraph
//! \return
//!
math::Graph ZWorld::getGraph() noexcept
{
    math::Graph graph(world_[0].size());

    for (uint32_t y = 0; y < world_.size(); y++){
        for (uint32_t x = 0; x < world_[0].size(); x++){

            auto ch = world_[y][x];

            if(isWayElement(ch)){

                std::list<zgame::point_type> neighbours;

                //neighbours
                if (x != 0 && isWayElement(world_[y][x - 1])){
                    //LEFT
                    neighbours.push_back({x - 1, y});
                }
                if (y != 0 && isWayElement(world_[y - 1][x])){
                    //UP
                    neighbours.push_back({x, y - 1});
                }
                if (y < (world_.size() - 1) && isWayElement(world_[y + 1][x])){
                    // DOWN
                    neighbours.push_back({x, y + 1});
                }
                if (x < (world_[0].size() - 1) && isWayElement(world_[y][x + 1])){
                    // RIGHT
                    neighbours.push_back({x + 1, y});

                }

                graph.addNode(zgame::point_type({x, y}), std::move(neighbours));
            }
        }
    }

    return graph;
}

bool ZWorld::isWayElement(char ch)
{
    return (ch == '-'  || ch == '.');
}

int ZWorld::getIndex(uint32_t x, uint32_t y)
{
    return (y * world_[0].size() + x);
}

std::size_t ZWorld::getWidth() const noexcept
{
    return world_[0].size();
}

std::size_t ZWorld::getHight() const noexcept
{
    return world_.size();
}

void ZWorld::load(std::fstream& file)
{
    if(!file.is_open())
        throw std::runtime_error("File is not opened");

    std::string line;
    while(file >> line){

        world_.emplace_back(line);
    }

    tiles_ = getTiles(world_);
}

void ZWorld::save(std::fstream& file)
{
    if(!file.is_open())
        throw std::runtime_error("File is not opened");

    for(const auto& line : world_){

        file << line << '\n';
    }
}

std::vector<ZWorld::tile_ptr_type> ZWorld::getTiles(const std::vector<std::string>& map)
{
    using namespace zgame;
    std::vector<tile_ptr_type> tiles;
    tiles.reserve(2000);

    // ..init rects..
    for(uint16_t y = 0; y < map.size(); ++y){
        for(uint16_t x = 0; x < map[y].size(); ++x){

            auto ch = map[y][x];
            switch (ch) {
            default:
                break;
            }

            tiles.emplace_back(
                        TILE_FACTORY.Create(
                            ch,
                            std::make_shared<zgame::rect>(
                                zgame::rect({ {x * TILE_SIZE, y * TILE_SIZE}, {TILE_SIZE, TILE_SIZE} })
                                ),
                            stats_->tiles,
                            stats_->rgb)
                        );
        }
    }

    return tiles;
}


