#pragma once

#include <string>
#include <vector>

enum LetterColour{
    GREY,
    YELLOW,
    GREEN
};

std::string pickWord();

std::string getInput();

std::array<LetterColour, 5> evaluateInput(std::string guess, std::string word);

void updateGuessedLetters(std::vector<std::tuple<char, LetterColour>> &guessedLetters, std::string guess, std::array<LetterColour, 5> result);

void displayGuess(std::array<LetterColour, 5> guess, std::string word);

bool playAgain();

bool checkWin(std::string guess, std::string word);