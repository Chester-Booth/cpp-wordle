#include <iostream>
#include <vector>
#include "logic.h"

void gameLoop(const Config& cfg, const std::unordered_set<std::string>& validInputWords) {

    // guessed letters vector
    std::vector<GuessedLetter> guessedLetters;

    // pick word
    std::string word = "hippy"; // pickWord(cfg.dataDir / "answer-words.txt");

    // output keyboard
    displayKeyboard(guessedLetters, MAX_GUESSES);

    // guess loop
    for (int i = 0; i < MAX_GUESSES; i++) {

        // get input + validate input
        std::string guess = getInput(validInputWords);

        // evaluate input
        std::array<LetterColour, WORD_LENGTH> result = evaluateInput(guess, word);

        // update guessed letters
        updateGuessedLetters(guessedLetters, guess, result);

        // display Guess
        displayGuess(result, guess);

        // output keyboard
        displayKeyboard(guessedLetters, (MAX_GUESSES - 1) - i);

        // check win
        if (checkWin(guess, word, (MAX_GUESSES - 1) - i)) {
            return;
        }
    }
    // loss
    displayLoss(word);
}

int main(int argc, char* argv[]) {

    // config
    Config cfg;
    cfg.dataDir = std::filesystem::canonical(argv[0]).parent_path().parent_path() / "data";

    // load word list for validating input, once at the start of the program, instead of every time
    // we get input
    std::unordered_set<std::string> validInputWords = loadWordList(cfg.dataDir / "input-words.txt");

    // game loop
    while (true) {
        gameLoop(cfg, validInputWords);

        // ask to play again
        if (!playAgain()) {
            return 0;
        } else {
            // clear console (move cursor up 10 lines and clear line, for the [MAX_GUESSES] guesses,
            // keyboard(3) and spacing(1))
            for (int i = 0; i < (KEYBOARD_LINE_COUNT + MAX_GUESSES); i++) {
                std::cout << ansi::CURSOR_UP << ansi::CLEAR_LINE << '\r';
            }
        }
    }
}
