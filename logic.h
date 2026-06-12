#pragma once

#include <array>
#include <tuple>
#include <string>
#include <vector>
#include <filesystem>

enum LetterColour{
    GREY,
    YELLOW,
    GREEN
};


struct Config {
    std::filesystem::path dataDir;
};

std::string pickWord(const Config& cfg);

std::string getInput(const Config& cfg, int errors = 0);

std::array<LetterColour, 5> evaluateInput(const std::string& guess, const std::string& word);

void updateGuessedLetters(std::vector<std::tuple<char, LetterColour>> &guessedLetters, std::string guess, std::array<LetterColour, 5> result);

void displayGuess(const std::array<LetterColour, 5>& guess,const std::string& word);

void displayKeyboard(const std::vector<std::tuple<char, LetterColour>>& guessedLetters, int remainingGuesses);

bool playAgain();

bool checkWin(const std::string& guess, const std::string& word, int remainingGuesses);