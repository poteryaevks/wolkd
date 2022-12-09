#pragma once

#include <fstream>

#include <boost/noncopyable.hpp>

#include "iobject.h"
#include "iinit.h"
#include "init_types.h"


namespace zgame {

struct PlayerStats;
struct ZombieStats;


//!
//! \brief The IInit class
//!
class JsInit // : public IInit
{

public:

    //!
    //! \brief Деструктор
    //!
    virtual ~JsInit() = default;

    //!
    //! \brief JsInit
    //! \param path
    //!
    JsInit(const std::string& path);

    //!
    //! \brief init
    //! \param object
    //!
    void init();

    //!
    //! \brief getPlayerData
    //! \return
    //!
    PlayerStats extractPlayerData();

    //!
    //! \brief getPlayerData
    //! \return
    //!
    std::list<ZombieStats> extractZombiesData();

    //!
    //! \brief extractWorldData
    //! \return
    //!
    WorldStats extractWorldData();

private:

    std::string path_;
    std::ifstream file_;
    PlayerStats playerInitData_;
    WorldStats worldInitData_;
    std::list<ZombieStats> zombiesInitData_;
};


}
