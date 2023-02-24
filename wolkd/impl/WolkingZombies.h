#pragma once

#include "../IGame.h"
#include "../IWorld.h"
#include "../IObject.h"

#include <sgraphics/engine/BaseGame.hpp>
#include <sgraphics/engine/IEvent.hpp>

namespace game
{
    class WolkingZombies final : public IGame, public sg::BaseGame
    {
    public:
        WolkingZombies(const std::string &title);
        ~WolkingZombies() override;

        void OnUpdate(const Duration &) override;
        void OnQuit() override;
        void OnCreate() override;

        const FRectType& GetPlayerRect() const noexcept override;
        FVectType GetOffset() const noexcept override;
        FRectRefs GetRects(ObjectsCategory category) const noexcept override;
        IdType GetPlayerId() const noexcept override;
        void OnEvent(Event::Ptr event) noexcept;

    private:
        void DrawPlayerStats();

    private:
        std::list<IObject::Ptr> m_zombies;
        IWorld::Ptr world_;
        IObject::Ptr player_;
        PointType defPosition_;
    };
}
