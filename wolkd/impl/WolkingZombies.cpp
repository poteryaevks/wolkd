#include "WolkingZombies.h"

#include <bfs/pathfinder.h>

#include "WorldImpl.h"
#include "Player.h"
#include "Zombie.h"
#include "JsInit.h"

#include "../Types.h"
#include "Stats.h"

#include <sgraphics/engine/Engine.hpp>

namespace
{
    int FULL_HP;
}

namespace game
{
    WolkingZombies::WolkingZombies(const std::string &title)
        : sg::BaseGame(title)
    {
        using namespace alm;

        // static IdType CurrentId{};
        auto eventer = sg::GetEngine().GetEventer();
        if (eventer)
        {
            eventer->AddEventHandler(*this, &WolkingZombies::InputEventHandler);
        }

        auto initializer = std::make_shared<JsInit>("assets/config.json");

        auto playerInitData = initializer->ExtractPlayerData();
        FULL_HP = playerInitData.hp;

        auto zombiesInitData = initializer->ExtractZombiesData();
        auto worldInitData = initializer->ExtractWorldData();

        /////////////////////////////map/////////////////////////////
        world_ = std::make_unique<WorldImpl>(this, std::make_shared<game::WorldStats>(std::move(worldInitData)));
        world_->addEventHandler(*this, &WolkingZombies::addEvent);

        /////////////////////////////player/////////////////////////////
        player_ = std::make_unique<Player>(this, std::make_shared<game::PlayerStats>(std::move(playerInitData)));
        player_->addEventHandler(*this, &WolkingZombies::addEvent);

        /////////////////////////////zombies/////////////////////////////
        for (auto &&zombieInitData : zombiesInitData)
        {
            auto zombie = std::make_unique<Zombie>(this, std::make_shared<ZombieStats>(std::move(zombieInitData)));
            zombie->addEventHandler(*this, &WolkingZombies::addEvent);
            m_zombies.push_back(std::move(zombie));
        }

        auto window = sg::GetEngine().GetWindow();

        const auto &screenW = window->GetWidth();
        const auto &screenH = window->GetHeight();
        defPosition_ = {(float)(screenW / 2), (float)(screenH / 2)};
    }

    WolkingZombies::~WolkingZombies() = default;

    void WolkingZombies::OnUpdate(const Duration &duration)
    {
        world_->Show(duration);

        for (auto &object : m_zombies)
        {
            object->Show(duration);
        }

        player_->Show(duration);

        printStats();
    }

    void WolkingZombies::OnQuit()
    {
    }

    PointType WolkingZombies::GetPlayerPos() const noexcept
    {
        return player_->GetRect()->pos;
    }

    FVectType WolkingZombies::GetOffset() const noexcept
    {
        return (defPosition_ - player_->GetRect()->pos);
    }

    std::vector<FRectPtr> WolkingZombies::GetRects(ObjectsCategory category) const noexcept
    {
        std::vector<FRectPtr> rects;

        switch (category)
        {
        case eObjectCategory::MAP:
        {
            auto worldRects = world_->GetRects();
            std::move(std::begin(worldRects), std::end(worldRects), std::back_inserter(rects));
            break;
        }
        case eObjectCategory::PLAYER:
        {
            rects.emplace_back(player_->GetRect());
            break;
        }
        case eObjectCategory::ENEMY:
        {
            for (const auto &object : m_zombies)
            {
                rects.push_back(object->GetRect());
            }
            break;
        }
        case eObjectCategory::MAP | eObjectCategory::PLAYER:
        {
            rects.emplace_back(player_->GetRect());
            auto worldRects = world_->GetRects();

            std::move(std::begin(worldRects), std::end(worldRects), std::back_inserter(rects));
            break;
        }
        case eObjectCategory::MAP | eObjectCategory::ENEMY:
        {
            for (const auto &object : m_zombies)
            {
                rects.push_back(object->GetRect());
            }

            auto worldRects = world_->GetRects();
            std::move(std::begin(worldRects), std::end(worldRects), std::back_inserter(rects));
            break;
        }
        case eObjectCategory::MAP | eObjectCategory::ENEMY | eObjectCategory::PLAYER:
        {
            for (const auto &object : m_zombies)
            {
                rects.push_back(object->GetRect());
            }

            rects.emplace_back(player_->GetRect());
            auto worldRects = world_->GetRects();

            std::move(std::begin(worldRects), std::end(worldRects), std::back_inserter(rects));
            break;
        }
        default:
            break;
        }

        return std::move(rects);
    }

    IdType WolkingZombies::GetPlayerId() const noexcept
    {
        return player_->GetId();
    }

    void WolkingZombies::addEvent(Event::Ptr event) noexcept
    {
        if (!event)
            return;

        switch (event->getType())
        {
        case ATTACK:
        {
            auto id = event->getId();

            if (player_->GetId() == id)
            {
                player_->OnAttack(event);
                return;
            }

            for (const auto &zombie : m_zombies)
            {
                if (zombie->GetId() == id)
                {
                    zombie->OnAttack(event);
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
        auto renderer = sg::GetEngine().GetRenderer();
        const auto &hp = player_->GetStats()->hp;

        renderer->DrawRect(FRectType({{20, 20}, {FULL_HP * 2, 20}}), {160, 150, 150, 150}, true);
        renderer->DrawRect(FRectType({{20, 20}, {hp * 2, 20}}), {190, 37, 37, 255}, true);
        renderer->DrawRect(FRectType({{20, 20}, {FULL_HP * 2, 20}}), {0, 0, 0, 255});
    }

    void WolkingZombies::InputEventHandler(sg::IEvent::Ptr event)
    {
        if (!event)
        {
            return;
        }

        switch (event->GetType())
        {
        case sg::EventType::MouseButtonDown:
        {
            auto xy = event->GetMousePosition();
            player_->OnInput(xy);
            break;
        }
        case sg::EventType::Quit:
        {
            quit_ = true;
            break;
        }
        default:
            break;
        }
    }
}
