#include <iostream>
#include <exception>

#include <sgraphics/engine/polygon.h>

#include "zgame.h"


//!
//!//! \brief main
//!//! \param argc
//!//! \param argv
//!//! \return
//!
int main(int argc, char ** argv)
{
    try{

        sg::engine::Polygon<WolkingZombies>::instance().init();
        sg::engine::Polygon<WolkingZombies>::instance().run();
    }
    catch(const std::exception& ex){

        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

