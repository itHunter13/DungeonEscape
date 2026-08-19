#include "Item.h"

Item::Item(
    const std::string& itemName,
    char itemSymbol,
    int itemValue)
    : name(itemName),
      symbol(itemSymbol),
      value(itemValue)
{
}

std::string Item::getName() const
{
    return name;
}

char Item::getSymbol() const
{
    return symbol;
}

int Item::getValue() const
{
    return value;
}