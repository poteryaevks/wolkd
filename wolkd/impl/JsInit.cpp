#include "JsInit.h"

#include "Zombie.h"
#include "Player.h"

#include <fstream>
#include <sstream>

#define RAPIDJSON_ASSERT(x) \
    assert(x)

#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace game
{
    JsInit::JsInit(const std::string &path)
    {
        std::ifstream file_(path);
        if (!file_)
        {
            throw std::runtime_error("Unable to open file");
        }

        std::ostringstream data;
        data << file_.rdbuf();

        rapidjson::Document doc;
        rapidjson::ParseResult parse_result = doc.Parse(data.str().c_str());
        if (parse_result.IsError())
        {
            std::string error("Unable to parse initial data ");
            error += rapidjson::GetParseError_En(parse_result.Code());
            throw std::runtime_error(error);
            return;
        }

        for (const auto &zombieValue : doc["game"]["zombies"].GetArray())
        {
            ZombieStats zombieStats;
            zombieStats.path = zombieValue["path"].GetString();
            zombieStats.speed = zombieValue["speed"].GetUint();
            zombieStats.type = (ZombieStats::eZombieType)zombieValue["type"].GetUint();
            zombieStats.dist = zombieValue["dist"].GetFloat();
            zombieStats.damage = zombieValue["damage"].GetUint();
            zombieStats.id = zombieValue["id"].GetUint();

            int i{0};
            for (const auto &value : zombieValue["rgb"].GetArray())
            {
                zombieStats.rgb[i++] = value.GetInt();
            }

            int j{0};
            for (const auto &value : zombieValue["position"].GetArray())
            {
                if (j++ == 0)
                    zombieStats.position.x = value.GetInt();
                else
                    zombieStats.position.y = value.GetInt();
            }
    
            zombiesStats_.emplace_back(std::move(zombieStats));
        }

        const auto &playerValue = doc["game"]["player"];
        playerStats_.path = playerValue["path"].GetString();
        playerStats_.speed = playerValue["speed"].GetUint();
        playerStats_.hp = playerValue["hp"].GetUint();
        playerStats_.id = playerValue["id"].GetUint();

        int i{0};
        for (const auto &value : playerValue["rgb"].GetArray())
        {
            playerStats_.rgb[i++] = value.GetInt();
        }

        int j{0};
        for (const auto &value : playerValue["position"].GetArray())
        {
            if (j++ == 0)
                playerStats_.position.x = value.GetInt();
            else
                playerStats_.position.y = value.GetInt();
        }

        const auto &worldValue = doc["game"]["world"];
        worldInitData_.tiles = worldValue["tiles"].GetString();
        worldInitData_.map = worldValue["map"].GetString();

        int k{0};
        for (const auto &value : worldValue["rgb"].GetArray())
        {
            worldInitData_.rgb[k++] = value.GetInt();
        }

        worldInitData_.id = worldValue["id"].GetUint();
    }

    JsInit::~JsInit() = default;

    PlayerStats JsInit::ExtractPlayerData()
    {
        return std::move(playerStats_);
    }

    std::list<ZombieStats> JsInit::ExtractZombiesData()
    {
        return std::move(zombiesStats_);
    }

    WorldStats JsInit::ExtractWorldData()
    {
        return std::move(worldInitData_);
    }
}
