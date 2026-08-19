#include "SaveManager.h"

#include <cstdio>
#include <fstream>
#include <string>

namespace
{
    const std::string SAVE_FILE = "dungeon_save.txt";
    const std::string SCORE_FILE = "dungeon_score.txt";
}

bool SaveManager::saveBestScore(
    int coins,
    int level,
    int kills)
{
    std::ofstream file(SCORE_FILE);

    if (!file)
        return false;

    file << coins << '\n';
    file << level << '\n';
    file << kills << '\n';

    return true;
}

bool SaveManager::loadBestScore(
    int& coins,
    int& level,
    int& kills)
{
    std::ifstream file(SCORE_FILE);

    if (!file)
    {
        coins = 0;
        level = 0;
        kills = 0;
        return false;
    }

    file >> coins >> level >> kills;

    return true;
}

bool SaveManager::saveGame(
    const Player& player,
    const std::vector<std::vector<Tile>>& map,
    const std::vector<Enemy>& enemies,
    Position exitPosition,
    Position keyPosition)
{
    std::ofstream file(SAVE_FILE);

    if (!file)
        return false;

    file << player.getLevel() << '\n';
    file << player.getExperience() << '\n';
    file << player.getCoins() << '\n';
    file << player.getPotions() << '\n';
    file << player.getKey() << '\n';
    file << player.getEnemiesKilled() << '\n';
    file << player.getHP() << '\n';
    file << player.getMaxHP() << '\n';
    file << player.getAttack() << '\n';

    file << player.position.x << ' '
         << player.position.y << '\n';

    file << exitPosition.x << ' '
         << exitPosition.y << '\n';

    file << keyPosition.x << ' '
         << keyPosition.y << '\n';

    for (const auto& row : map)
    {
        for (Tile tile : row)
            file << static_cast<int>(tile) << ' ';

        file << '\n';
    }

    file << enemies.size() << '\n';

    for (const Enemy& enemy : enemies)
    {
        file << enemy.getName() << '\n';
        file << enemy.getHP() << '\n';
        file << enemy.getMaxHP() << '\n';
        file << enemy.getAttack() << '\n';
        file << enemy.position.x << ' '
             << enemy.position.y << '\n';
    }

    return true;
}

bool SaveManager::loadGame(
    Player& player,
    std::vector<std::vector<Tile>>& map,
    std::vector<Enemy>& enemies,
    Position& exitPosition,
    Position& keyPosition)
{
    std::ifstream file(SAVE_FILE);

    if (!file)
        return false;

    int level;
    int experience;
    int coins;
    int potions;
    int key;
    int kills;
    int hp;
    int maxHp;
    int attack;

    file >> level
         >> experience
         >> coins
         >> potions
         >> key
         >> kills
         >> hp
         >> maxHp
         >> attack;

    file >> player.position.x
         >> player.position.y;

    file >> exitPosition.x
         >> exitPosition.y;

    file >> keyPosition.x
         >> keyPosition.y;

    map.assign(
        HEIGHT,
        std::vector<Tile>(WIDTH, Tile::Empty));

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            int tile;
            file >> tile;
            map[y][x] = static_cast<Tile>(tile);
        }
    }

    size_t enemyCount;
    file >> enemyCount;

    enemies.clear();

    for (size_t i = 0; i < enemyCount; ++i)
    {
        std::string name;
        int enemyHp;
        int enemyMaxHp;
        int enemyAttack;
        Position position;

        file.ignore();
        std::getline(file, name);

        file >> enemyHp
             >> enemyMaxHp
             >> enemyAttack;

        file >> position.x >> position.y;

        Enemy enemy(
            name,
            enemyMaxHp,
            enemyAttack,
            position);

        enemy.setHP(enemyHp);
        enemies.push_back(enemy);
    }

    player.setLevel(level);
    player.setExperience(experience);
    player.setCoins(coins);
    player.setPotions(potions);
    player.setKey(key);
    player.setEnemiesKilled(kills);
    player.setMaxHP(maxHp);
    player.setAttack(attack);
    player.setHP(hp);

    return true;
}

bool SaveManager::saveExists() const
{
    std::ifstream file(SAVE_FILE);
    return file.good();
}

bool SaveManager::deleteSave()
{
    return std::remove(SAVE_FILE.c_str()) == 0;
}