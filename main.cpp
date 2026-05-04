#include "RPG_Project.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << fixed << setprecision(0);

    Game game;
    game.start();
    return 0;
}
