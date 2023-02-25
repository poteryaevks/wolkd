#pragma once

#include "../IGame.h"
#include "../IWorld.h"
#include "../IObject.h"

#include <sgraphics/engine/BaseGame.hpp>
#include <sgraphics/engine/IEvent.hpp>
#include <sgraphics/engine/IEventer.hpp>

namespace game
{
    class WolkingZombies final : public IGame, public sgraphics::BaseGame
    {
    public:
        WolkingZombies(const std::string &title);
        ~WolkingZombies() override;

        void OnUpdate(const Duration &) override;
        void OnQuit() override;
        void OnCreate() override;

        void OnEvent(Event::Ptr event) noexcept;
        const FRectType& GetPlayerRect() const noexcept override;
        FVectType GetOffset() const noexcept override;
        FRectRefs GetRects(ObjectsCategory category) const noexcept override;
        IdType GetPlayerId() const noexcept override;

    private:
        void DrawPlayerStats();

    private:
        std::list<IObject::Ptr> m_zombies;
        sgraphics::IEventer::Ptr sgEventer_;
        IObject::Ptr player_;
        IWorld::Ptr world_;
        PointType defPosition_;
    };
}
