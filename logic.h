#pragma once

#include <array>
#include <tuple>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_set>

inline constexpr int WORD_LENGTH = 5;
inline constexpr int MAX_GUESSES = 6;
inline constexpr int KEYBOARD_LINE_COUNT = 4;

namespace ansi {
constexpr auto CLEAR_LINE = "\u001b[2K";
constexpr auto CURSOR_UP = "\u001b[1A";
constexpr auto CURSOR_DOWN = "\u001b[1B";
constexpr auto RESET = "\u001b[0m";
constexpr auto BG_GREY = "\u001b[100m";
constexpr auto BG_YELLOW = "\u001b[43m";
constexpr auto BG_GREEN = "\u001b[42m";
constexpr auto FG_BLACK = "\u001b[30m";
} // namespace ansi

enum LetterColour {
    GREY = 0, // worst
    YELLOW = 1,
    GREEN = 2 // best
};

struct Config {
    std::filesystem::path dataDir;
};

struct GuessedLetter {
    char letter;
    LetterColour colour;
};

std::unordered_set<std::string> loadWordList(const std::filesystem::path& path);

std::string pickWord(const Config& cfg);

std::string getInput(const Config& cfg, const std::unordered_set<std::string>& validInputWords,
                     int errors = 0);

std::array<LetterColour, WORD_LENGTH> evaluateInput(const std::string& guess,
                                                    const std::string& word);

void updateGuessedLetters(std::vector<GuessedLetter>& guessedLetters, const std::string& guess,
                          const std::array<LetterColour, WORD_LENGTH>& result);

void displayGuess(const std::array<LetterColour, WORD_LENGTH>& colours, const std::string& guess);

void displayKeyboard(const std::vector<GuessedLetter>& guessedLetters, int remainingGuesses);

bool playAgain();

bool checkWin(const std::string& guess, const std::string& word, int remainingGuesses);