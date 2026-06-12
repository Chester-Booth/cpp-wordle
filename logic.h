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

std::array<LetterColour, 5> evaluateInput(std::string guess, std::string word);

void updateGuessedLetters(std::vector<std::tuple<char, LetterColour>> &guessedLetters, std::string guess, std::array<LetterColour, 5> result);

void displayGuess(std::array<LetterColour, 5> guess, std::string word);

void displayKeyboard(std::vector<std::tuple<char, LetterColour>> guessedLetters, int remainingGuesses);

bool playAgain();

bool checkWin(std::string guess, std::string word, int remainingGuesses);