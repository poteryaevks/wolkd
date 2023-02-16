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
    int FULL_HP{};
}

namespace game
{
    WolkingZombies::WolkingZombies(const std::string &title)
        : sg::BaseGame(title)
    {
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

        world_ = std::make_unique<WorldImpl>(this, std::move(worldInitData));
        world_->addEventHandler(*this, &WolkingZombies::OnEvent);

        player_ = std::make_unique<Player>(this, std::move(playerInitData));
        player_->addEventHandler(*this, &WolkingZombies::OnEvent);

        for (auto &&zombieInitData : zombiesInitData)
        {
            auto zombie = std::make_unique<Zombie>(this, std::move(zombieInitData));
            zombie->addEventHandler(*this, &WolkingZombies::OnEvent);
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

        DrawPlayerStats();
    }

    void WolkingZombies::OnQuit() {}

    const FRectType& WolkingZombies::GetPlayerRect() const noexcept
    {
        return player_->GetRect();
    }

    FVectType WolkingZombies::GetOffset() const noexcept
    {
        auto pointer = dynamic_cast<Player *>(player_.get());
        return (defPosition_ - pointer->offset_.pos);
    }

    FRectRefs WolkingZombies::GetRects(ObjectsCategory category) const noexcept
    {
        FRectRefs result;

        if (category & eObjectCategory::MAP)
        {
            const auto &rects = world_->GetRects();

            for (const auto &rect : rects)
            {
                result.push_back(rect);
            }
        }

        if (category & eObjectCategory::ENEMY)
        {
            for (const auto &object : m_zombies)
            {
                result.push_back(object->GetRect());
            }
        }

        if (category & eObjectCategory::PLAYER)
        {
            result.push_back(player_->GetRect());
        }

        return result;
    }

    IdType WolkingZombies::GetPlayerId() const noexcept
    {
        return player_->GetId();
    }

    void WolkingZombies::OnEvent(Event::Ptr event) noexcept
    {
        if (!event)
            return;

        switch (event->getType())
        {
        case ATTACK:
        {
            auto id = event->GetId();

            if (player_->GetId() == id)
            {
                player_->OnEvent(event);
                return;
            }

            for (const auto &zombie : m_zombies)
            {
                if (zombie->GetId() == id)
                {
                    zombie->OnEvent(event);
                    return;
                }
            }

            break;
        }
        default:
            break;
        }
    }

    void WolkingZombies::DrawPlayerStats()
    {
        auto renderer = sg::GetEngine().GetRenderer();
        const auto &hp = player_->GetStats().hp;

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
