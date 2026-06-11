#include <string> 
#include <fstream> 
#include <cstdlib> 
#include <iostream>
#include <algorithm>
#include <cctype>

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

    int randomNum = rand() % lineCount+1;

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