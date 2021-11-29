#include "Game.hpp"

int main()
{
    try
    {
        Game{}.run();
    }
    catch (String ex)
    {
        Game::logError(ex);
        return -1;
    }

    return 0;
}
