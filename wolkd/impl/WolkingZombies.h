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

        PointType GetPlayerPos() const noexcept override;
        FVectType GetOffset() const noexcept override;
        std::vector<FRectPtr> GetRects(ObjectsCategory category) const noexcept override;
        IdType GetPlayerId() const noexcept override;
        void addEvent(Event::Ptr event) noexcept;
        void InputEventHandler(sg::IEvent::Ptr event);

    private:
        void printStats();

    private:
        std::list<IObject::Ptr> m_zombies;
        IWorld::Ptr world_;
        IObject::Ptr player_;
        PointType defPosition_;
    };
}
