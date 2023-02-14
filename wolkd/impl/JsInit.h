#pragma once

#include "../IInit.h"
#include "Stats.h"

namespace game
{
    class JsInit final : public IInit
    {
    public:
        ~JsInit() override;
        JsInit(const std::string &path);
        PlayerStats ExtractPlayerData() override;
        std::list<ZombieStats> ExtractZombiesData() override;
        WorldStats ExtractWorldData() override;

    private:
        PlayerStats playerStats_;
        WorldStats worldInitData_;
        std::list<ZombieStats> zombiesStats_;
    };
}
