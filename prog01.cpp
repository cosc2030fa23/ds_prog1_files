#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "timer.h"  // Include your timer header file

// Define the Node structure for the linked list
template <typename T>
struct Node {
    T data;
    Node<T>* next;
};

// Define the LinkedList class
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    void insert(const T& data) {
        Node<T>* newNode = new Node<T>{data, nullptr};
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        ++size;
    }

    bool find(const T& data, int& compares) {
        Node<T>* temp = head;
        compares = 0;
        while (temp != nullptr) {
            ++compares;
            if (temp->data.compare(data) == 0) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    int getSize() {
        return size;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

// Clean word function
std::string cleanWord(const std::string& word) {
    std::string cleaned;
    for (char c : word) {
        if (std::isalpha(c) || c == '\'') {
            cleaned += std::tolower(c);
        }
    }
    return cleaned;
}

int main() {
    LinkedList<std::string> dictionary;
    std::ifstream dictFile("dict.txt");
    std::string word;

    // Load dictionary into linked list
    while (dictFile >> word) {
        dictionary.insert(cleanWord(word));
    }
    dictFile.close();

    std::cout << "dictionary size " << dictionary.getSize() << std::endl;

    // Initialize variables for spell checking
    int correctWords = 0, incorrectWords = 0, skippedWords = 0;
    int totalComparesCorrect = 0, totalComparesIncorrect = 0;
    int compares;

    std::ofstream misspelledFile("misspelled.txt");

    std::ifstream bookFile("book.txt");

    Timer timer;  // Initialize the timer
    timer.Start();  // Start the timer

    // Spell check each word in the book
    while (bookFile >> word) {
        std::string cleaned = cleanWord(word);
        if (cleaned.empty()) {
            continue;
        }
        if (!std::isalpha(cleaned[0])) {
            ++skippedWords;
            continue;
        }
        if (dictionary.find(cleaned, compares)) {
            ++correctWords;
            totalComparesCorrect += compares;
        } else {
            ++incorrectWords;
            totalComparesIncorrect += compares;
            misspelledFile << word << std::endl;
        }
    }

    timer.Stop();  // Stop the timer
    double duration = timer.Time();  // Get the elapsed time

    bookFile.close();
    misspelledFile.close();


    std::cout << "Done checking and these are the results" << std::endl;
    std::cout << "finished in time: " << duration << std::endl;
    std::cout << "There are " << correctWords << " words found in the dictionary" << std::endl;
    std::cout << " " << totalComparesCorrect << " compares. Average: " << (totalComparesCorrect / correctWords) << std::endl;
    std::cout << "There are " << incorrectWords << " words NOT found in the dictionary" << std::endl;
    std::cout << " " << totalComparesIncorrect << " compares. Average: " << (totalComparesIncorrect / incorrectWords) << std::endl;
    std::cout << "There are " << skippedWords << " words not checked." << std::endl;

    return 0;
}
