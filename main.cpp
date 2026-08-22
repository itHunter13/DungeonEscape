#include "Game.h"

#include <clocale>

int main()
{
    // Настройка локализации для отображения текста.
    setlocale(LC_ALL, "");

    Game game;
    game.start();

    return 0;
}