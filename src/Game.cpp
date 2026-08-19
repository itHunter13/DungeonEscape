#include "Game.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

Game::Game()
{
    generator.seed(
        static_cast<unsigned>(time(nullptr))
    );

    map.resize(
        HEIGHT,
        vector<Tile>(WIDTH, Tile::Empty)
    );

    gameOver = false;
    victory = false;

    createItems();
}

void Game::createItems()
{
    items.clear();

    items.emplace_back(
        "Зелье",
        'P',
        30
    );

    items.emplace_back(
        "Монета",
        '$',
        10
    );

    items.emplace_back(
        "Ключ",
        'K',
        1
    );
}
void Game::generateMap()
{
    // ...
}

bool Game::isInside(Position position) const
{
    // ...
}

bool Game::isFree(Position position) const
{
    // ...
}

int Game::randomInt(int min, int max)
{
    // ...
}
void Game::gameLoop()
{
    // ...
}

void Game::start()
{
    // ...
}