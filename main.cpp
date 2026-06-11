#include <iostream>
#include <array>
#include "logic.h"


int main(int argc, char *argv[]){
    // game loop
    while(true){
        // pick word
        std::string word = pickWord();

        // guess loop
        for (int i = 0; i < 6; i++){

            // get input + validate input
            std::string guess = getInput();
            
            // evaluate input
            std::array<LetterColour, 5> result = evaluateInput(guess, word);
            
            
            // output board
            
            // ouput keyboard

            // check win 
        }
        // loss 
        std::cout << "You Loose :(\nThe word was: " << word << std::endl;

        // ask to play again

    }
    return 0;

} 

