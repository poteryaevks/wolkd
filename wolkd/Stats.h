#pragma once

#include "Types.h"

#include <memory>
#include <filesystem>

namespace game
{
    struct Stats
    {
        using Ptr = std::shared_ptr<Stats>;
        virtual ~Stats() = default;
        sgraphics::RgbType rgb;
        std::filesystem::path path;
        float damage;
        float speed;
        IdType id;
        PointType position;
        int hp;
    };

    struct ZombieStats : public Stats
    {
        ZombieStats() = default;
        enum eZombieType
        {
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
        float dist;
    };

    struct PlayerStats : public Stats
    {
        PlayerStats() = default;
    };

    struct WorldStats : public Stats
    {
        WorldStats() = default;
        std::string tiles;
        std::string map;
    };
}
