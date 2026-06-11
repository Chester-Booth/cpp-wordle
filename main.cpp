#include <iostream>
#include <array>
#include <tuple>
#include <vector>
#include "logic.h"

void gameLoop(){
    // guessed letters vector
    std::vector<std::tuple<char, LetterColour>> guessedLetters;

    // pick word
    std::string word = pickWord();

    // output keyboard
    displayKeyboard(guessedLetters, 6);

    // guess loop
    for (int i = 0; i < 6; i++){

        // get input + validate input
        std::string guess = getInput();
        
        // evaluate input
        std::array<LetterColour, 5> result = evaluateInput(guess, word);

        // update guessed letters
        updateGuessedLetters(guessedLetters, guess, result);

        // display Guess
        displayGuess(result, guess);
        
        // output keyboard
        displayKeyboard(guessedLetters, 5 - i);

        
        // check win 
        if (checkWin(guess,word,5 - i)){return;}
    }
    // loss 
    std::cout << "You Loose :(\nThe word was: " << word << "\n\n";
}

int main(int argc, char *argv[]){
    projectRoot = std::filesystem::canonical(argv[0]).parent_path().parent_path();

    // game loop
    while (true){
        gameLoop();
  
        // ask to play again
        if (!playAgain()){
            return 0;
        }
        else{
            // clear console (move cursor up 10 lines and clear line, for the 6 guesses, keyboard and spacing)
            for (int i = 0; i < 10; i++){
                std::cout << "\u001b[1A\u001b[2K\r";
            }
        }
    }
} 
