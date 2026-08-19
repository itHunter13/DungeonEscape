#pragma once

#include <string>

class Item
{
private:
    std::string name;
    char symbol;
    int value;

public:
    Item(
        const std::string& itemName,
        char itemSymbol,
        int itemValue);

    std::string getName() const;
    char getSymbol() const;
    int getValue() const;
};