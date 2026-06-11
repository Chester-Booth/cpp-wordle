#include "logic.h"
#include <string> 
#include <fstream> 
#include <iostream>
#include <algorithm>
#include <cctype>
#include <array>
#include <random>
#include <tuple>

// helper functions

int countLines(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    int count = 0;

    while (std::getline(file, line)) {
        ++count;
    }
    return count;
}

bool wordInList(const std::string& word, const std::string& path) {
    std::ifstream file(path);
    std::string line;
    std::string lowerWord = word;

    // lowercase
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

    while (std::getline(file, line)) {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        if (line == lowerWord) {
            return true;
        }
    }
    return false;
}



// public questions

std::string pickWord() {
    std::ifstream MyReadFile("data/answer-words.txt");
    int lineCount = countLines("data/answer-words.txt");

    // seed 
    std::random_device rd; 
    // init random number generator with seed
    std::mt19937 gen(rd()); 
    // create distribution for line numbers
    std::uniform_int_distribution<> distr(1, lineCount);
    // pick random line number
    int randomNum = distr(gen);

    std::string word;
    for (int i = 0; i <= randomNum && std::getline(MyReadFile, word); i++) {}

    return word;
}

std::string getInput() {
    std::string guess;

    std::cout << "Guess: ";
    std::cin >> guess;

    if (guess.length() != 5) {
        std::cout << "Invalid input, try again.\n";
        return getInput();
    }
    else if (wordInList(guess, "data/input-words.txt") == false) {
        std::cout << "Word not in list, try again.\n";
        return getInput();
    }
    else
    {
        return guess;
    }
}

std::array<LetterColour, 5> evaluateInput(std::string guess, std::string word){
    // lowercase both words 
    std::string lowerWord = word;
    std::string lowerGuess = guess;
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    std::transform(lowerGuess.begin(), lowerGuess.end(), lowerGuess.begin(), ::tolower);

    std::array<LetterColour, 5> result = {GREY, GREY, GREY, GREY, GREY};

    // first pass for greens, remove from guess to avoid double counting
    for (int i = 0; i < 5; i++){
        if (lowerGuess[i] == lowerWord[i]){
            result[i] = GREEN;
            lowerWord[i] = '_'; // prevent double counting for yellow
        }
    }

    // yellows, letter in word, but not green
    for (int i = 0; i < 5; i++){
        if (result[i] == GREEN){
            continue; // already green, skip
        }

        if (lowerWord.find(lowerGuess[i]) != std::string::npos){
            result[i] = YELLOW;
            // remove first instance of letter from lowerWord to prevent double counting
            int pos = lowerWord.find(lowerGuess[i]);
            if (pos != std::string::npos) {
                lowerWord[pos] = '_';
            }
        }
    }
    

    return result;
}

void updateGuessedLetters(
    std::vector<std::tuple<char, LetterColour>> &guessedLetters, 
    std::string guess, std::array<LetterColour, 5> result
){
   for (int i = 0; i < 5; i++){

    char currentGuess = guess[i];
    LetterColour currentResult = result[i];

        // get the tuple in guessedLetters with char currentGuess, if it exists
        std::vector<std::tuple<char, LetterColour>>::iterator foundLetter = std::find_if(guessedLetters.begin(), guessedLetters.end(),
        [currentGuess](const std::tuple<char, LetterColour>& element) {
        return std::get<0>(element) == currentGuess;
    });


        // if it exists, update the colour if currentResult is better (GREEN > YELLOW > GREY)
        if (foundLetter != guessedLetters.end()) {
            // already exists — update
            if (currentResult > std::get<1>(*foundLetter)) {
                // update tuple in guessedLetters with currentResult
                std::get<1>(*foundLetter) = currentResult;
        }
        } else {
            // add to guessedLetters
            guessedLetters.emplace_back(currentGuess, currentResult);
    }
    }

}

void displayGuess(std::array<LetterColour, 5> guess, std::string word){
    
    // display guess with colours (ansi codes)
    for (int i = 0; i < 5; i++){
        switch (guess[i]){
            case GREY:
                std::cout << "\u001b[30m\u001b[100m" << word[i];
                break;
            case YELLOW:
                std::cout << "\u001b[30m\u001b[43m" << word[i];
                break;
            case GREEN:
                std::cout << "\u001b[30m\u001b[42m" << word[i];
                break;
        }
    }

    // reset colour + newline
    std::cout << "\u001b[0m\n";

}

bool playAgain(){
    std::string input;
    std::cout << "Play again? (y/n): ";
    std::cin >> input;
    std::string lowerInput = input;

    // lowercase
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);


    if (lowerInput[0] == 'y'){
        return true;
    }
    else if (lowerInput[0] == 'n'){
        return false;
    }
    else{
        std::cout << "Invalid input, try again.\n";
        return playAgain();
    }
}

bool checkWin(std::string guess, std::string word){
    // lowercase both words 
    std::string lowerWord = word;
    std::string lowerGuess = guess;
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    std::transform(lowerGuess.begin(), lowerGuess.end(), lowerGuess.begin(), ::tolower);
            
    if (lowerGuess == lowerWord){
        std::cout << "You Win!\nThe word was: " << word << std::endl;
        return true;
    }
    else{
        return false;
    }
    
}
