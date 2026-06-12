#include "logic.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <random>
#include <vector>

// helper functions

static bool wordInList(const std::string& word, const std::unordered_set<std::string>& validWords) {
    // check if in set
    return validWords.count(word) > 0;
}

// public functions

std::unordered_set<std::string> loadWordList(const std::filesystem::path& path) {
    std::unordered_set<std::string> words;
    std::ifstream file(path);

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            // lowercase
            std::transform(line.begin(), line.end(), line.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            // add to set
            words.insert(line);
        }
    }
    file.close();

    return words;
}

std::string pickWord(const std::filesystem::path& path) {

    // open filestram
    std::ifstream file(path);

    // setup vars
    std::vector<std::string> words;
    std::string line;

    // add each (non-empty) line to vector
    while (std::getline(file, line)) {
        if (!line.empty())
            words.push_back(line);
    }
    if (words.empty())
        throw std::runtime_error("answer word list is empty or missing");
    file.close();

    // seed
    std::random_device rd;
    // init random number generator with seed
    std::mt19937 gen(rd());
    // create distribution for line numbers
    std::uniform_int_distribution<> distr(0, words.size() - 1);
    // pick random line number
    int randomNum = distr(gen);

    // return lowercased word at that line
    std::string word = words[randomNum];
    std::transform(word.begin(), word.end(), word.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return word;
}

std::string getInput(const std::unordered_set<std::string>& validInputWords) {
    bool error = false;

    while (true) {
        std::string guess;
        if (!(std::cin >> guess))
            return "";

        std::cout << ansi::CURSOR_UP << ansi::CLEAR_LINE
                  << '\r'; // clear line and return cursor to start

        // if there were errors, clear previous error messages
        if (error) {
            // clear line and return custor to start
            std::cout << ansi::CURSOR_UP << ansi::CLEAR_LINE << '\r';
            error = false; // reset error after clearing
        }

        std::string lowerGuess = guess;

        // lowercase
        std::transform(lowerGuess.begin(), lowerGuess.end(), lowerGuess.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (guess.length() != WORD_LENGTH) {
            std::cout << "Invalid input, try again.\n";
            error = true;
        } else if (wordInList(lowerGuess, validInputWords) == false) {
            std::cout << "Word not in list, try again.\n";
            error = true;
        } else {
            return lowerGuess;
        }
    }
}

std::array<LetterColour, WORD_LENGTH> evaluateInput(const std::string& guess,
                                                    const std::string& word) {
    // copy of word for manipulation
    std::string alteredWord = word;

    std::array<LetterColour, WORD_LENGTH> result = {GREY, GREY, GREY, GREY, GREY};

    // first pass for greens, remove from guess to avoid double counting
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == word[i]) {
            result[i] = GREEN;
            alteredWord[i] = '_'; // prevent double counting for yellow
        }
    }

    // yellows, letter in word, but not green
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] == GREEN) {
            continue; // already green, skip
        }

        // remove first instance of letter from lowerWord to prevent double counting
        std::string::size_type pos = alteredWord.find(guess[i]);
        if (pos != std::string::npos) {
            result[i] = YELLOW;
            alteredWord[pos] = '_';
        }
    }

    return result;
}

void updateGuessedLetters(std::vector<GuessedLetter>& guessedLetters, const std::string& guess,
                          const std::array<LetterColour, WORD_LENGTH>& result) {
    for (int i = 0; i < WORD_LENGTH; i++) {

        char currentGuess = guess[i];
        LetterColour currentResult = result[i];

        // get the tuple in guessedLetters with char currentGuess, if it exists
        std::vector<GuessedLetter>::iterator foundLetter =
            std::find_if(guessedLetters.begin(), guessedLetters.end(),
                         [currentGuess](const GuessedLetter& element) {
                             return element.letter == currentGuess;
                         });

        // if it exists, update the colour if currentResult is better (GREEN > YELLOW > GREY)
        if (foundLetter != guessedLetters.end()) {
            // already exists — update
            if (currentResult > foundLetter->colour) {
                // update tuple in guessedLetters with currentResult
                foundLetter->colour = currentResult;
            }
        } else {
            // add to guessedLetters
            guessedLetters.emplace_back(GuessedLetter{currentGuess, currentResult});
        }
    }
}

void displayGuess(const std::array<LetterColour, WORD_LENGTH>& colours, const std::string& guess) {

    // display guess with colours (ansi codes)
    for (int i = 0; i < WORD_LENGTH; i++) {
        switch (colours[i]) {
        case GREY:
            std::cout << ansi::FG_BLACK << ansi::BG_GREY << guess[i];
            break;
        case YELLOW:
            std::cout << ansi::FG_BLACK << ansi::BG_YELLOW << guess[i];
            break;
        case GREEN:
            std::cout << ansi::FG_BLACK << ansi::BG_GREEN << guess[i];
            break;
        }
    }

    // reset colour + newline
    std::cout << ansi::RESET << '\n';
}

void displayKeyboard(const std::vector<GuessedLetter>& guessedLetters, int remainingGuesses) {
    // add newlines equal to remaining guesses to move keyboard down
    for (int i = 0; i < remainingGuesses; i++) {
        std::cout << "\n";
    }

    // initial newline for spacing (and errors on final guess) and qwerty layout
    std::string keyboard = "\nq w e r t y u i o p\n  a s d f g h j k l\n    z x c v b n m";
    // for every char in keyboard set as c
    for (char& c : keyboard) {

        // get the tuple in guessedLetters with char currentGuess, if it exists
        std::vector<GuessedLetter>::const_iterator foundLetter =
            std::find_if(guessedLetters.begin(), guessedLetters.end(),
                         [c](const GuessedLetter& element) { return element.letter == c; });

        // if in guessedLetters, display background colour
        if (foundLetter != guessedLetters.end()) {
            switch (foundLetter->colour) {
            case GREY:
                std::cout << ansi::FG_BLACK << ansi::BG_GREY << c << ansi::RESET;
                break;
            case YELLOW:
                std::cout << ansi::FG_BLACK << ansi::BG_YELLOW << c << ansi::RESET;
                break;
            case GREEN:
                std::cout << ansi::FG_BLACK << ansi::BG_GREEN << c << ansi::RESET;
                break;
            }
        } else {
            std::cout << c;
        }
    }
    // move cursor up to past the keyboard and to the next guess and return to start
    for (int i = 0; i < (KEYBOARD_LINE_COUNT + (remainingGuesses - 1)); i++) {
        std::cout << ansi::CURSOR_UP;
    }
    std::cout << '\r';
}

bool checkWin(const std::string& guess, const std::string& word, int remainingGuesses) {
    if (guess == word) {
        // move cursor down to below keyboard and output win message
        for (int i = 0; i <= remainingGuesses; i++) {
            std::cout << ansi::CURSOR_DOWN << ansi::CLEAR_LINE;
        }

        std::cout << "You Win!\nThe word was: " << word << std::endl;
        return true;
    } else {
        return false;
    }
}

bool playAgain() {
    std::string input;

    while (true) {
        std::cout << "Play again? (y/n): ";

        if (!(std::cin >> input))
            return false;

        std::string lowerInput = input;

        // lowercase
        std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (lowerInput.empty()) {
            return false;
        }
        if (lowerInput[0] == 'y') {
            return true;
        } else if (lowerInput[0] == 'n') {
            return false;
        } else {
            std::cout << "Invalid input, try again.\n";
        }
    }
}

void displayLoss(const std::string& word) {
    // ouput loss message and clear leftover lines from the keyboard
    std::cout << "You Lose :(\nThe word was: " << word << '\n'
              << ansi::CLEAR_LINE << '\n'
              << ansi::CLEAR_LINE;
}