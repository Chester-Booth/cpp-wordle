#include <iostream>
#include <array>
#include "logic.h"

void gameLoop(){
    // pick word
    std::string word = pickWord();

    // guess loop
    for (int i = 0; i < 6; i++){

        // get input + validate input
        std::string guess = getInput();
        
        // evaluate input
        std::array<LetterColour, 5> result = evaluateInput(guess, word);
        

        // display Guess
        displayGuess(result, guess);
        
        // ouput keyboard
        

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
