#include "zgame.h"

#include <sgraphics/engine/polygon_funcs.h>

#include <bfs/pathfinder.h>

#include "zmap_object.h"
#include "player_object.h"
#include "zombie_object.h"
#include "jsinit.h"
#include "init_types.h"


namespace {

int FULL_HP;

}

void WolkingZombies::initImpl()
{
    using namespace alm;

    static zgame::id_type currentId{1};

    auto initializer = std::make_shared<zgame::JsInit>("assets/config.json");
    initializer->init();

    auto playerInitData = initializer->extractPlayerData();
    FULL_HP = playerInitData.hp;

    auto zombiesInitData = initializer->extractZombiesData();

    auto worldInitData = initializer->extractWorldData();

    /////////////////////////////map/////////////////////////////
    world_ = std::make_unique<ZWorld>(
                shared_from_this(),
                std::make_shared<zgame::WorldStats>(std::move(worldInitData))
                );

    world_->addEventHandler(*this, &WolkingZombies::addEvent);

    /////////////////////////////player/////////////////////////////
    player_ = std::make_unique<ZPlayer>(
                shared_from_this(),
                std::make_shared<zgame::PlayerStats>(std::move(playerInitData))
                );

    player_->addEventHandler(*this, &WolkingZombies::addEvent);

    /////////////////////////////zombies/////////////////////////////
    for(auto&& zombieInitData : zombiesInitData){

        auto npc = std::make_unique<ZZombie>(
                    shared_from_this(),
                    std::make_shared<zgame::ZombieStats>(std::move(zombieInitData))
                    );

        npc->addEventHandler(*this, &WolkingZombies::addEvent);
        enemies.push_back(
                    std::move(npc)
                    );
    }

    static const int32_t screenW = sg::engine::getWidth();
    static const int32_t screenH = sg::engine::getHeight();
    defPosition_ = { (float)(screenW / 2), (float)(screenH / 2) };
}


void WolkingZombies::updateImpl(float timePoint)
{
    sg::engine::clearScreen({255, 255, 255, 255});

    world_->show(timePoint);

    for(auto& object : enemies){

        object->show(timePoint);
    }

    player_->show(timePoint);

    printStats();
}

void WolkingZombies::quitImpl()
{
}


zgame::point_type WolkingZombies::getPlayerPos() const noexcept
{
    return player_->getRect()->pos;
}


math::Graph WolkingZombies::getGraph() noexcept
{
//    auto it = std::find_if(enemies.begin(),
//                           enemies.end(),
//                           [](enemy_ptr_type& object)
//    { return object->getCategory() == zgame::MAP; });

//    if(it == enemies.end())
//        return math::Graph();

//    ZWorld* map = dynamic_cast<ZWorld*>(it->get());
//    if(map == nullptr)
//        return math::Graph();

//    return map->getGraph();
    return math::Graph();
}


zgame::vect_type WolkingZombies::getOffset() const noexcept
{
    return (defPosition_ - player_->getRect()->pos);
}


std::vector<std::shared_ptr<zgame::rect>> WolkingZombies::getRects(ObjectsCategory category) const noexcept
{
    std::vector<std::shared_ptr<zgame::rect>> rects;

    switch (category)
    {
    case zgame::MAP:{

        auto worldRects = world_->getRects();

        std::move(std::begin(worldRects),
                  std::end(worldRects),
                  std::back_inserter(rects)
                  );
        break;
    }
    case zgame::PLAYER:
    {
        rects.emplace_back(player_->getRect());
        break;

    }
    case zgame::ENEMY:
    {
        for(const auto& object : enemies){

            rects.push_back(
                        object->getRect()
                        );
        }
        break;
    }
    case zgame::MAP | zgame::PLAYER:
    {
        rects.emplace_back(player_->getRect());

        auto worldRects = world_->getRects();

        std::move(std::begin(worldRects),
                  std::end(worldRects),
                  std::back_inserter(rects)
                  );

        break;
    }
    case zgame::MAP | zgame::ENEMY:
    {
        for(const auto& object : enemies){

            rects.push_back(
                        object->getRect()
                        );
        }

        auto worldRects = world_->getRects();

        std::move(std::begin(worldRects),
                  std::end(worldRects),
                  std::back_inserter(rects)
                  );

        break;
    }
    case zgame::MAP | zgame::ENEMY | zgame::PLAYER:
    {
        for(const auto& object : enemies){

            rects.push_back(
                        object->getRect()
                        );
        }

        rects.emplace_back(player_->getRect());
        auto worldRects = world_->getRects();

        std::move(std::begin(worldRects),
                  std::end(worldRects),
                  std::back_inserter(rects)
                  );

        break;
    }
    default:
        break;
    }

    return std::move(rects);
}

zgame::id_type WolkingZombies::getPlayerId() const noexcept
{
    return player_->getId();
}


void WolkingZombies::addEvent(std::shared_ptr<zgame::Event> msg) noexcept
{
    if(msg == nullptr)
        return;

    switch (msg->getType())
    {
    case zgame::ATTACK:
    {
        auto id = msg->getId();

        if(player_->getId() == id){

            player_->getAttacked(msg);
            return;
        }

        for(const auto& enemy : enemies){

            if(enemy->getId() == id){

                enemy->getAttacked(msg);
                return;
            }
        }

        break;
    }
    default:
        break;
    }
}


void WolkingZombies::printStats()
{
    sg::engine::drawRect(
                zgame::rect({{ 20, 20 },{ FULL_HP * 2, 20 }}),
    { 160, 150, 150, 150 },
                true
                );

    auto hp = player_->getStats()->hp;
    sg::engine::drawRect(
                zgame::rect({{ 20, 20 },{ hp * 2, 20 }}),
    { 190, 37, 37, 255 },
                true
                );

    sg::engine::drawRect(
                zgame::rect({{ 20, 20 },{ FULL_HP * 2, 20 }}),
    { 0, 0, 0, 255 }
                );
}
