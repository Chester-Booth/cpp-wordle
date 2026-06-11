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
        
        // ouput keyboard
        displayKeyboard(guessedLetters);

        
        // check win 
        if (checkWin(guess,word)){return;}
    }
    // loss 
    std::cout << "You Loose :(\nThe word was: " << word << std::endl;
}

int main(int argc, char *argv[]){
    // game loop
    while (true){
        gameLoop();
  
        // ask to play again
        if (!playAgain()){
            return 0;
        }
    }
} 
