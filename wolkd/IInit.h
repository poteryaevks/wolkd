#pragma once

#include <boost/noncopyable.hpp>

#include <list>
#include <memory>
#include "Stats.h"

namespace game
{
    class IInit : private boost::noncopyable
    {
    public:
        using Ptr = std::shared_ptr<IInit>;
        virtual ~IInit() = default;
        virtual PlayerStats ExtractPlayerData() = 0;
        virtual std::list<ZombieStats> ExtractZombiesData() = 0;
        virtual WorldStats ExtractWorldData() = 0;
    };
}
