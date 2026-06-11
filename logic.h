#pragma once

#include <string>

enum LetterColour{
    GREY,
    YELLOW,
    GREEN
};

std::string pickWord();

std::string getInput();

std::array<LetterColour, 5> evaluateInput(std::string guess, std::string word);

