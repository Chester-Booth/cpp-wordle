#include <iostream>
#include <vector>
#include "logic.h"

void gameLoop(const Config& cfg, const std::unordered_set<std::string>& validInputWords){

    // guessed letters vector
    std::vector<GuessedLetter> guessedLetters;

    // pick word
    std::string word = pickWord(cfg);

    // output keyboard
    displayKeyboard(guessedLetters, MAX_GUESSES);

    // guess loop
    for (int i = 0; i < MAX_GUESSES; i++){

        // get input + validate input
        std::string guess = getInput(cfg, validInputWords);
        
        // evaluate input
        std::array<LetterColour, WORD_LENGTH> result = evaluateInput(guess, word);

        // update guessed letters
        updateGuessedLetters(guessedLetters, guess, result);

        // display Guess
        displayGuess(result, guess);
        
        // output keyboard
        displayKeyboard(guessedLetters, (MAX_GUESSES - 1 ) - i);

        
        // check win 
        if (checkWin(guess,word,5 - i)){return;}
    }
    // loss 
    std::cout << "You Loose :(\nThe word was: " << word << "\nz\u001b[2K\n";
}

int main(int argc, char *argv[]){

    // config
    Config cfg;
    cfg.dataDir = std::filesystem::canonical(argv[0]).parent_path().parent_path() / "data";

    // load word list for validating input, once at the start of the program, instead of every time we get input
    std::unordered_set<std::string> validInputWords = loadWordList(cfg.dataDir / "input-words.txt");

    // game loop
    while (true){
        gameLoop(cfg, validInputWords);
  
        // ask to play again
        if (!playAgain()){
            return 0;
        }
        else{
            // clear console (move cursor up 10 lines and clear line, for the [MAX_GUESSES] guesses, keyboard(3) and spacing(1))
            for (int i = 0; i < (4+MAX_GUESSES); i++){
                std::cout << "\u001b[1A\u001b[2K\r";
            }
        }
    }
} 
