#include "Game.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

Game::Game()
{
    generator.seed(static_cast<unsigned>(time(nullptr)));

    map.resize(HEIGHT, vector<Tile>(WIDTH, Tile::Empty));

    gameOver = false;
    victory = false;

    createItems();
}

void Game::createItems()
{
    items.clear();

    items.emplace_back("Зелье", 'P', 30);
    items.emplace_back("Монета", '$', 10);
    items.emplace_back("Ключ", 'K', 1);
}

void Game::generateMap()
{
    map.assign(HEIGHT, vector<Tile>(WIDTH, Tile::Empty));
    enemies.clear();

    // Создаём границу подземелья.
    for (int y = 0; y < HEIGHT; ++y)
    {
        map[y][0] = Tile::Wall;
        map[y][WIDTH - 1] = Tile::Wall;
    }

    for (int x = 0; x < WIDTH; ++x)
    {
        map[0][x] = Tile::Wall;
        map[HEIGHT - 1][x] = Tile::Wall;
    }

    // Несколько внутренних стен. Оставляем проходы, чтобы карта всегда
    // оставалась удобной для прохождения.
    for (int x = 4; x < WIDTH - 4; x += 6)
    {
        for (int y = 3; y < HEIGHT - 3; ++y)
        {
            if (y != 7)
                map[y][x] = Tile::Wall;
        }
    }

    player.setPosition({1, 1});
    player.setKey(false);
    keyPosition = {WIDTH - 3, 2};
    exitPosition = {WIDTH - 2, HEIGHT - 2};

    map[keyPosition.y][keyPosition.x] = Tile::Key;
    map[exitPosition.y][exitPosition.x] = Tile::Exit;

    // Предметы размещаем только на свободных клетках.
    int potionCount = 3;
    int coinCount = 8;

    while (potionCount > 0)
    {
        Position p{randomInt(2, WIDTH - 3), randomInt(2, HEIGHT - 3)};

        if (isFree(p) && p != keyPosition && p != exitPosition)
        {
            map[p.y][p.x] = Tile::Potion;
            --potionCount;
        }
    }

    while (coinCount > 0)
    {
        Position p{randomInt(2, WIDTH - 3), randomInt(2, HEIGHT - 3)};

        if (isFree(p) && p != keyPosition && p != exitPosition)
        {
            map[p.y][p.x] = Tile::Coin;
            --coinCount;
        }
    }

    const vector<string> enemyTypes = {"Goblin", "Orc", "Skeleton"};

    for (int i = 0; i < 4; ++i)
    {
        Position p{randomInt(3, WIDTH - 3), randomInt(2, HEIGHT - 3)};

        if (!isFree(p) || p == player.position ||
            p == keyPosition || p == exitPosition)
        {
            --i;
            continue;
        }

        const string& type = enemyTypes[i % enemyTypes.size()];
        const int hp = 35 + i * 10;
        const int attack = 8 + i * 2;

        enemies.emplace_back(type, hp, attack, p);
    }
}

bool Game::isInside(Position position) const
{
    return position.x >= 0 && position.x < WIDTH &&
           position.y >= 0 && position.y < HEIGHT;
}

bool Game::isFree(Position position) const
{
    if (!isInside(position))
        return false;

    return map[position.y][position.x] != Tile::Wall;
}

int Game::randomInt(int min, int max)
{
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void Game::draw()
{
    cout << "\n";
    cout << "========== DUNGEON ESCAPE ==========\n";

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            Position current{x, y};
            char symbol = '.';

            if (current == player.position)
            {
                symbol = player.getSymbol();
            }
            else
            {
                const int enemyIndex = findEnemy(current);

                if (enemyIndex != -1)
                    symbol = enemies[enemyIndex].getSymbol();
                else
                {
                    switch (map[y][x])
                    {
                    case Tile::Wall:   symbol = '#'; break;
                    case Tile::Potion: symbol = 'P'; break;
                    case Tile::Coin:   symbol = '$'; break;
                    case Tile::Key:    symbol = 'K'; break;
                    case Tile::Exit:   symbol = 'X'; break;
                    case Tile::Empty:  symbol = '.'; break;
                    }
                }
            }

            cout << symbol;
        }

        cout << '\n';
    }

    cout << "\nHP: " << player.getHP() << "/" << player.getMaxHP()
         << " | LVL: " << player.getLevel()
         << " | EXP: " << player.getExperience()
         << " | Coins: " << player.getCoins()
         << " | Potions: " << player.getPotions()
         << " | Key: " << (player.getKey() ? "YES" : "NO")
         << " | Kills: " << player.getEnemiesKilled() << '\n';

    cout << "WASD - движение | H - зелье | I - информация | G - сохранить\n";
    cout << "L - загрузить | ? - помощь | Q - выйти\n";
}

void Game::movePlayer(int dx, int dy)
{
    Position newPosition{
        player.position.x + dx,
        player.position.y + dy
    };

    if (!isInside(newPosition) || !isFree(newPosition))
    {
        cout << "\nНельзя пройти через стену или выйти за карту.\n";
        return;
    }

    const int enemyIndex = findEnemy(newPosition);

    if (enemyIndex != -1)
    {
        attackEnemy(enemyIndex);
        return;
    }

    player.setPosition(newPosition);
    collectItem();
    checkExit();
}

void Game::collectItem()
{
    const Tile tile = map[player.position.y][player.position.x];

    if (tile == Tile::Potion)
    {
        player.addPotion();
        map[player.position.y][player.position.x] = Tile::Empty;
        cout << "\nТы нашёл зелье!\n";
    }
    else if (tile == Tile::Coin)
    {
        player.addCoins(10);
        map[player.position.y][player.position.x] = Tile::Empty;
        cout << "\nТы нашёл монету! +10\n";
    }
    else if (tile == Tile::Key)
    {
        player.obtainKey();
        map[player.position.y][player.position.x] = Tile::Empty;
        cout << "\nТы нашёл ключ! Теперь можно открыть выход.\n";
    }
}

void Game::checkExit()
{
    if (player.position == exitPosition)
    {
        if (player.getKey())
        {
            victory = true;
            gameOver = true;
            cout << "\nТы нашёл выход и выбрался из подземелья!\n";
        }
        else
        {
            cout << "\nВыход закрыт. Сначала найди ключ!\n";
        }
    }
}

int Game::findEnemy(Position position)
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        if (enemies[i].position == position && enemies[i].isAlive())
            return static_cast<int>(i);
    }

    return -1;
}

void Game::attackEnemy(int enemyIndex)
{
    if (enemyIndex < 0 ||
        enemyIndex >= static_cast<int>(enemies.size()))
        return;

    Enemy& enemy = enemies[enemyIndex];

    cout << "\nТы атакуешь " << enemy.getName()
         << " и наносишь " << player.getAttack() << " урона.\n";

    enemy.takeDamage(player.getAttack());

    if (!enemy.isAlive())
    {
        cout << enemy.getName() << " побеждён!\n";

        player.addEnemyKill();
        player.addExperience(50);
        player.addCoins(20);

        enemies.erase(enemies.begin() + enemyIndex);
        return;
    }

    cout << enemy.getName() << " атакует в ответ и наносит "
         << enemy.getAttack() << " урона.\n";

    player.takeDamage(enemy.getAttack());

    if (!player.isAlive())
    {
        gameOver = true;
        victory = false;
        cout << "\nТвоё здоровье закончилось. Игра окончена.\n";
    }
}

void Game::enemyTurn()
{
    // В текущей версии враги атакуют только при прямом столкновении.
    // Это делает управление предсказуемым и подходит для консольной игры.
}

void Game::showInfo()
{
    cout << "\n========== ИНФОРМАЦИЯ ==========\n";
    cout << "Имя: " << player.getName() << '\n';
    cout << "Роль: " << player.getRole() << '\n';
    cout << "Описание: " << player.getDescription() << '\n';
    cout << "Уровень: " << player.getLevel() << '\n';
    cout << "HP: " << player.getHP() << "/" << player.getMaxHP() << '\n';
    cout << "Атака: " << player.getAttack() << '\n';
    cout << "Опыт: " << player.getExperience() << '\n';
    cout << "Монеты: " << player.getCoins() << '\n';
    cout << "Зелья: " << player.getPotions() << '\n';
    cout << "Врагов побеждено: " << player.getEnemiesKilled() << '\n';
    cout << "Ключ: " << (player.getKey() ? "есть" : "нет") << '\n';
}

void Game::showHelp()
{
    cout << "\n========== ПОМОЩЬ ==========\n";
    cout << "W - вверх\n";
    cout << "S - вниз\n";
    cout << "A - влево\n";
    cout << "D - вправо\n";
    cout << "H - использовать зелье\n";
    cout << "I - информация о герое\n";
    cout << "G - сохранить игру\n";
    cout << "L - загрузить игру\n";
    cout << "? - показать помощь\n";
    cout << "Q - выйти из игры\n";
    cout << "\nЧтобы победить: найди K и дойди до X.\n";
}

void Game::showPolymorphismDemo()
{
    cout << "\n========== ПОЛИМОРФИЗМ ==========\n";

    vector<const Character*> characters;
    characters.push_back(&player);

    for (const Enemy& enemy : enemies)
        characters.push_back(&enemy);

    for (const Character* character : characters)
    {
        cout << character->getRole()
             << " | "
             << character->getSymbol()
             << " | "
             << character->getDescription()
             << '\n';
    }

    cout << "\nОдин интерфейс Character работает с Player и Enemy.\n";
}

void Game::saveGame()
{
    if (saveManager.saveGame(
            player,
            map,
            enemies,
            exitPosition,
            keyPosition))
    {
        cout << "\nИгра сохранена.\n";
    }
    else
    {
        cout << "\nОшибка: не удалось сохранить игру.\n";
    }
}

bool Game::loadGame()
{
    if (!saveManager.loadGame(
            player,
            map,
            enemies,
            exitPosition,
            keyPosition))
    {
        cout << "\nСохранение не найдено или повреждено.\n";
        return false;
    }

    cout << "\nИгра загружена.\n";
    return true;
}

void Game::updateBestScore()
{
    int oldCoins = 0;
    int oldLevel = 0;
    int oldKills = 0;

    saveManager.loadBestScore(oldCoins, oldLevel, oldKills);

    const int currentScore =
        player.getCoins() +
        player.getLevel() * 100 +
        player.getEnemiesKilled() * 50;

    const int oldScore =
        oldCoins +
        oldLevel * 100 +
        oldKills * 50;

    if (currentScore > oldScore)
    {
        saveManager.saveBestScore(
            player.getCoins(),
            player.getLevel(),
            player.getEnemiesKilled());

        cout << "\nНовый лучший результат!\n";
    }
}

void Game::showBestScore()
{
    int coins = 0;
    int level = 0;
    int kills = 0;

    if (!saveManager.loadBestScore(coins, level, kills))
    {
        cout << "\nЛучший результат ещё не установлен.\n";
        return;
    }

    const int score = coins + level * 100 + kills * 50;

    cout << "\n========== ЛУЧШИЙ РЕЗУЛЬТАТ ==========\n";
    cout << "Очки: " << score << '\n';
    cout << "Уровень: " << level << '\n';
    cout << "Монеты: " << coins << '\n';
    cout << "Побеждено врагов: " << kills << '\n';
}

void Game::showFinalScreen()
{
    cout << "\n====================================\n";

    if (victory)
    {
        cout << "             ПОБЕДА!\n";
        cout << "Ты выбрался из подземелья.\n";
    }
    else
    {
        cout << "            ИГРА ОКОНЧЕНА\n";
    }

    cout << "Уровень: " << player.getLevel() << '\n';
    cout << "Монеты: " << player.getCoins() << '\n';
    cout << "Врагов побеждено: "
         << player.getEnemiesKilled() << '\n';

    updateBestScore();
    showBestScore();

    cout << "====================================\n";
}

int Game::mainMenu()
{
    while (true)
    {
        cout << "\n========== DUNGEON ESCAPE ==========\n";
        cout << "1. Новая игра\n";
        cout << "2. Загрузить игру\n";
        cout << "3. Лучший результат\n";
        cout << "4. Выход\n";
        cout << "Выберите пункт: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите число от 1 до 4.\n";
            continue;
        }

        if (choice >= 1 && choice <= 4)
            return choice;

        cout << "Ошибка: выберите число от 1 до 4.\n";
    }
}

void Game::gameLoop()
{
    while (!gameOver)
    {
        draw();

        cout << "\nВаш ход: ";

        char command;
        cin >> command;

        command = static_cast<char>(
            tolower(static_cast<unsigned char>(command)));

        switch (command)
        {
        case 'w':
            movePlayer(0, -1);
            break;

        case 's':
            movePlayer(0, 1);
            break;

        case 'a':
            movePlayer(-1, 0);
            break;

        case 'd':
            movePlayer(1, 0);
            break;

        case 'h':
            player.usePotion();
            break;

        case 'i':
            showInfo();
            break;

        case 'g':
            saveGame();
            break;

        case 'l':
            loadGame();
            break;

        case '?':
            showHelp();
            break;

        case 'q':
            gameOver = true;
            victory = false;
            cout << "\nИгра завершена.\n";
            break;

        default:
            cout << "\nНеизвестная команда. Нажми ? для помощи.\n";
            break;
        }

        if (!gameOver)
            enemyTurn();
    }

    showFinalScreen();
}

void Game::start()
{
    while (true)
    {
        const int choice = mainMenu();

        if (choice == 1)
        {
            player = Player();
            gameOver = false;
            victory = false;
            generateMap();
            gameLoop();
            return;
        }

        if (choice == 2)
        {
            if (loadGame())
            {
                gameOver = false;
                victory = false;
                gameLoop();
                return;
            }
        }
        else if (choice == 3)
        {
            showBestScore();
        }
        else if (choice == 4)
        {
            cout << "\nДо встречи!\n";
            return;
        }
    }
}
