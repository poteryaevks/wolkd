#include "jsinit.h"
#include "zombie_object.h"
#include "player_object.h"

#include <sstream>


#define RAPIDJSON_ASSERT(x) \
    assert(x)

#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace  {

rapidjson::Document doc;

}


namespace zgame {

JsInit::JsInit(const std::string& path)
    : path_(path)
{
    file_.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    file_.open(path);
    if(!file_.is_open()){

        throw std::runtime_error("Unable to open file");
    }
}


void JsInit::init()
{
    std::ostringstream data;
    data << file_.rdbuf();

    rapidjson::ParseResult parse_result = doc.Parse(data.str().c_str());
    if(parse_result.IsError()){

        std::string error("Unable to parse initial data ");
        error += rapidjson::GetParseError_En(parse_result.Code());
        throw std::runtime_error(error);
        return;
    }

    for(const rapidjson::Value& zombieValue : doc["zgame"]["zombies"].GetArray()){

        ZombieStats zombieInitData;
        zombieInitData.path = zombieValue["path"].GetString();
        zombieInitData.speed = zombieValue["speed"].GetUint();
        zombieInitData.type = (ZombieStats::eZombieType)zombieValue["type"].GetUint();
        zombieInitData.anger = (ZombieStats::eAnger)zombieValue["anger"].GetUint();
        zombieInitData.damage = zombieValue["damage"].GetUint();
        zombieInitData.id = zombieValue["id"].GetUint();

        int i{0};
        for(const rapidjson::Value& value : zombieValue["rgb"].GetArray()){

            zombieInitData.rgb[i++] = value.GetInt();
        }


        int j{0};
        for(const rapidjson::Value& value : zombieValue["position"].GetArray()){

            if(j++ == 0)
                zombieInitData.position.x = value.GetInt();
            else
                zombieInitData.position.y = value.GetInt();
        }

        zombiesInitData_.emplace_back(
                    std::move(zombieInitData)
                    );
    }

    const rapidjson::Value& playerValue = doc["zgame"]["player"];
    playerInitData_.path = playerValue["path"].GetString();
    playerInitData_.speed = playerValue["speed"].GetUint();
    playerInitData_.hp = playerValue["hp"].GetUint();
    playerInitData_.id = playerValue["id"].GetUint();

    int i{0};
    for(const rapidjson::Value& value : playerValue["rgb"].GetArray()){

        playerInitData_.rgb[i++] = value.GetInt();
    }

    int j{0};
    for(const rapidjson::Value& value : playerValue["position"].GetArray()){

        if(j++ == 0)
            playerInitData_.position.x = value.GetInt();
        else
            playerInitData_.position.y = value.GetInt();
    }

    const rapidjson::Value& worldValue = doc["zgame"]["world"];
    worldInitData_.tiles = worldValue["tiles"].GetString();
    worldInitData_.map = worldValue["map"].GetString();

    int k{0};
    for(const rapidjson::Value& value : worldValue["rgb"].GetArray()){

        worldInitData_.rgb[k++] = value.GetInt();
    }

    worldInitData_.id = worldValue["id"].GetUint();
}

PlayerStats JsInit::extractPlayerData()
{
    return std::move(playerInitData_);
}

std::list<ZombieStats> JsInit::extractZombiesData()
{
    return std::move(zombiesInitData_);
}

WorldStats JsInit::extractWorldData()
{
    return std::move(worldInitData_);
}

}
