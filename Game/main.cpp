#include <iostream>
#include <OpenGL/Game/Game.h>

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::exception &e) {
        std::wclog << e.what() << std::endl;
        return -1;
    }

    return 0;
}