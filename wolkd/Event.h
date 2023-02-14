#pragma once

#include <Types.h>

namespace game
{
    enum EventType
    {
        OUT_OF_SCREEN,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        DEAD,
        ATTACK,
    };

    class Event
    {
    public:
        using Ptr = std::shared_ptr<Event>;
        virtual ~Event() = default;
        void SetId(IdType id) { id_ = id; }
        EventType getType() const { return type_; }
        IdType GetId() const { return id_; }

    protected:
        EventType type_;
        IdType id_;
    };

    class OutOfScreenEvent final : public Event
    {
    public:
        OutOfScreenEvent()
            : Event()
        {
            type_ = EventType::OUT_OF_SCREEN;
        }

        ~OutOfScreenEvent() = default;
        void setOrient(eOrientation orientation) { orientation_ = orientation; }
        eOrientation getOrient() const { return orientation_; }

    private:
        eOrientation orientation_;
    };

    class AttackEvent final : public Event
    {
    public:
        static std::shared_ptr<AttackEvent> create()
        {
            return std::make_shared<AttackEvent>();
        }

        AttackEvent()
            : Event()
        {
            type_ = EventType::ATTACK;
        }

        ~AttackEvent() = default;
        void setDamage(uint damage) { damage_ = damage; }
        uint getDamage() const { return damage_; }

    private:
        uint damage_;
    };
}
