#pragma once

#include <string>

class FXCard
{
private:
    std::string type;

public:
    // Constructor
    FXCard(std::string card_type) : type{card_type} {}

    std::string get_type()
    {
        return type;
    }
};