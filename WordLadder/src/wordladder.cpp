/*WorldLadder C++ Program:
 * CS 106B Assignment 2A
 * Author: Adriel Sumathipala
 * This program calculates the shortest distance between two words using words
 * in a user-supplied dictionary to build the word ladder.
 */

//Standard C++ Libraries:
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

//Stanford C++ Libraries:
#include "console.h"
#include "string.h"
#include "stack.h"
#include "filelib.h"
#include "simpio.h"
#include "lexicon.h"
#include "queue.h"

using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";

//Function prototypes:
void getWords(int &flag, string &word1, string &word2, Lexicon &dictionary);
void wordLadder(Queue<Stack<string>> &queue, Stack<string> &correctLadder,
                string &word1, string &word2, Lexicon &dictionary, int &printFlag);
void printStack(Stack<string> &stack, int &printFlag);
bool containsNonAlphabet(string word);


//main() function:
int main() {
    cout << "Welcome to CS 106B Word Ladder." <<endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    //Read in dictionary.txt file and save as Lexicon object:
    string fileName = getLine("Dictionary file name? ");
    ifstream stream;
    while(!openFile(stream, fileName)){
        cout << "Unable to open that file. Try again." << endl;
        fileName = getLine("Grid input file name? ");
    }
    Lexicon dictionary;
    dictionary.addWordsFromFile(stream);

    string word1;
    string word2;
    int flag = 0;
    int printFlag = 0;
    getWords(flag, word1, word2, dictionary);
    while(flag==0){
        Queue<Stack<string>> queue;
        Stack<string> correctLadder;
        wordLadder(queue, correctLadder, word1, word2, dictionary, printFlag);
        printStack(correctLadder, printFlag);
        getWords(flag, word1, word2, dictionary);
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/*getWords function:
 * Retrieves user's input words and checks if input
 * contains non-alphabetic characters, words not dictionary,
 * that the words are same length, and that the words are different.
 * Makes word1 and word2 lowercase and returns them by reference.
 */
void getWords(int &flag, string &word1, string &word2, Lexicon &dictionary){
    //Get words from user and confirm input formatted correctly:
    word1 = toLowerCase(getLine("Word #1 (or Enter to quit):"));
    if(word1 == "") {
        flag = 1;
        return;
    }
    if(!containsNonAlphabet(word1)){
        cout << "Please only enter alphabetic characters" << endl;
        getWords(flag, word1, word2, dictionary);
        return;
    }
    word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
    if(word2 == ""){
        flag = 1;
        return;
    }

    if(!containsNonAlphabet(word2)){
        cout << "Please only enter alphabetic characters" << endl;
        getWords(flag, word1, word2, dictionary);
        return;
    }

    if(word1.length() != word2.length()){
        cout << "The two words must be the same length." <<endl;
        getWords(flag, word1, word2, dictionary);
        return;
    }

    if(!dictionary.contains(word1) || !dictionary.contains(word2)){
        cout << "The two words must be found in the dictionary." <<endl;
        cout << endl;
        getWords(flag, word1, word2, dictionary);
        return;

    }

    if(word1 == word2){
        cout << "The two words must be different." << endl;
        cout << endl;
        getWords(flag, word1, word2, dictionary);
        return;
    }
    flag = 0;
    return;
}

/*containsNonAlphabet function:
 * Returns true if input string contains
 * non-alphabetic characters and false
 * if string contains only alphabetic characters.
 * Used in getWords() function
 */

bool containsNonAlphabet(string word){
    for(char c : word){
        if(!isalpha(c)){
            return isalpha(c);
        }
    }
    return true;
}

/* wordLadder Function:
 * Find shortest distance path between word1 and word2 using
 * breadth-first search (BFS). BFS ladder builds the ladder by adding "steps"
 * where each step is the previous word with one letter changed. Each partial
 * ladder is stored in a stack of strings and each stack is stored in a
 * queue of stacks.
 */
void wordLadder(Queue<Stack<string>> &queue, Stack<string> &correctLadder,
                string &word1, string &word2, Lexicon &dictionary, int &printFlag){
    // String that temporarily stores each "step" (word) in the ladder
    string step;
    //String that stores previous "step" in the ladder
    //Initialized to word1, which is first step in ladder
    string prevStep = word1;
    //Generate stack for word ladder:
    Stack<string> ladder;
    //Push word1 to stack:
    ladder.push(word1);
    //Add this stack to the queue:
    queue.enqueue(ladder);
    //Lexicon object to store words used in previous ladders:
    Lexicon usedWords;

    string::size_type word1Length = word1.length();
    do{
        //Remove
        //cout << queue.size() <<endl;
        //cout << "Iteration = "<< iter << endl;

        ladder = queue.peek();
        prevStep = ladder.peek();
        for(string::size_type i = 0; i < word1Length; i++){
            for(string::size_type j = 0; j < ALPHABET.length(); j++){
                //Create neigbors word
                step = ladder.peek();
                step.replace(i, 1, charToString(ALPHABET[j]));
                //Adds new step to stack if word contained in dictionary:
                if(dictionary.contains(step) && step != prevStep && !usedWords.contains(step)){
                    Stack<string> newLadder = ladder;
                    newLadder.push(step);
                    usedWords.add(step);
//                    if(ladder.size() < 5){
//                        int tmpFlag = 0;
//                        printStack(newLadder, tmpFlag);
//                    }
                    //Adds new ladder to end of queue:
                    queue.enqueue(newLadder);
                    if(step == word2){
                        correctLadder = newLadder;
                        printFlag = 0;
                        return;
                    }
                }
            }
        }
        queue.dequeue();

        //Remove
//        iter++;
    }while(!queue.isEmpty());
    //If queue is empty, then impossible to build word ladder:
    cout << "Unable to build word ladder" << endl;
    cout << endl;
    printFlag = 1;
    return;
}

/*printStack Function
 *Passes in stack by reference and prints out
 *stack contents to the console, without
 *modifying the stack passed in by reference.
 */
void printStack(Stack<string> &stack, int &printFlag){
    cout<< endl;
    if(printFlag == 1){
        return;
    } else{
        Stack<string> stackCopy = stack;
        do{
            cout << stackCopy.peek() << " ";
            stackCopy.pop();
        } while(!stackCopy.isEmpty());
        cout << endl;
    }
}

