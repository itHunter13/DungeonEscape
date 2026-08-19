#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Tile.h"

#include <vector>

class SaveManager
{
private:
    static constexpr int WIDTH = 30;
    static constexpr int HEIGHT = 15;

public:
    bool saveBestScore(int coins, int level, int kills);
    bool loadBestScore(int& coins, int& level, int& kills);

    bool saveGame(
        const Player& player,
        const std::vector<std::vector<Tile>>& map,
        const std::vector<Enemy>& enemies,
        Position exitPosition,
        Position keyPosition);

    bool loadGame(
        Player& player,
        std::vector<std::vector<Tile>>& map,
        std::vector<Enemy>& enemies,
        Position& exitPosition,
        Position& keyPosition);

    bool saveExists() const;
    bool deleteSave();
};