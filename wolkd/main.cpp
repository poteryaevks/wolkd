#include <iostream>
#include <exception>

#include "impl/WolkingZombies.h"

int main(int argc, char **argv)
{
    try
    {
        game::WolkingZombies game("Zombies");
        game.Run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
