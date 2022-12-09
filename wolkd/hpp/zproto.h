#pragma once

#include <ztypes.h>


namespace zgame{

enum eEventTypes{

    OUT_OF_SCREEN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    DEAD,
    ATTACK,
};


//!
//! \brief The ZMessage class
//!
class Event
{

public:

    virtual ~Event() = default;

    //!
    //! \brief setId
    //! \param type
    //!
    void setId(id_type id) { id_ = id; }

    //!
    //! \brief getType
    //! \return
    //!
    eEventTypes getType() const { return type_; }

    //!
    //! \brief setId
    //! \return
    //!
    id_type getId() const { return id_; }
protected:


    //!
    //! \brief ZMessage
    //!
    Event() = default;

    eEventTypes type_;
    id_type id_;
};

//!
//! \brief The ZOutOfScreenMsg class
//!
class OutOfScreenMsg final : public Event
{

public:

    OutOfScreenMsg()
        : Event()
    {
        type_ = eEventTypes::OUT_OF_SCREEN;
    }


    ~OutOfScreenMsg() = default;

    //!
    //! \brief setOrient
    //! \param orientation
    //!
    void setOrient(eOrientation orientation) { orientation_ = orientation; }

    //!
    //! \brief getOrient
    //! \return
    //!
    eOrientation getOrient() const { return orientation_; }

private:

    eOrientation orientation_;
};

//!
//! \brief The ZOutOfScreenMsg class
//!
class AttackEvent final : public Event
{
public:

    static std::shared_ptr<AttackEvent> create() {

        return std::make_shared<AttackEvent>();
    }

    AttackEvent()
        : Event()
    {
          type_ = eEventTypes::ATTACK;
    }

    ~AttackEvent() = default;

    void setDamage(uint damage) { damage_ = damage; }

    //!
    //! \brief getOrient
    //! \return
    //!
    uint getDamage() const { return damage_; }


private:

    uint damage_;
};

}
