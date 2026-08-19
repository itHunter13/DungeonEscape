#include "Game.h"

#include <clocale>

int main()
{
    // Настраиваем локализацию для корректного отображения русского текста.
    setlocale(LC_ALL, "");

    Game game;
    game.start();

    return 0;
}