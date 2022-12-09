#pragma once

#include "ztypes.h"

//!
//!
//!
namespace zgame{

struct Stats
{
    std::array<uint8_t, 3> rgb;
    std::string path;
    uint damage;
    uint speed;
    zgame::id_type id;
    zgame::point_type position;
    int hp;
};

struct ZombieStats : public Stats
{
    enum eAnger{
        neutral,
        angry,
        very_angry
    };

    enum eZombieType{
        type_1 = 0,
        type_2 = 1,
        type_3 = 2,
        type_4 = 3,
        type_5 = 4,
        type_6 = 5,
        type_7 = 6,
        type_8 = 7
    };

    eZombieType type;
    eAnger anger;
};

struct PlayerStats : public Stats
{
};

struct WorldStats : public Stats
{
    std::string tiles;
    std::string map;
};

}



