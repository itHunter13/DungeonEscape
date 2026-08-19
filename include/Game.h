#include "Position.h"
#pragma once

#include "Enemy.h"
#include "Item.h"
#include "Player.h"
#include "Position.h"
#include "SaveManager.h"
#include "Tile.h"

#include <random>
#include <vector>

class Game
{
private:
    static constexpr int WIDTH = 30;
    static constexpr int HEIGHT = 15;

    std::vector<std::vector<Tile>> map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Item> items;

    Position exitPosition;
    Position keyPosition;

    std::mt19937 generator;
    SaveManager saveManager;

    bool gameOver;
    bool victory;

    void createItems();
    void generateMap();

    bool isInside(Position position) const;
    bool isFree(Position position) const;

    int randomInt(int min, int max);

    void draw();
    void movePlayer(int dx, int dy);
    void collectItem();
    void checkExit();

    int findEnemy(Position position);
    void attackEnemy(int enemyIndex);
    void enemyTurn();

    void showInfo();
    void showHelp();
    void showPolymorphismDemo();

    void saveGame();
    bool loadGame();

    void updateBestScore();
    void showBestScore();
    void showFinalScreen();

    int mainMenu();
    void gameLoop();

public:
    Game();
    void start();
};