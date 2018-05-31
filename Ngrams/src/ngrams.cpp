/* N-gram Random Writer Program
 * CS 106B Assignment 2: Part B
 * Author: Adriel Sumathipala
 * This program takes in a text file and then randomly generates new text
 * in a similar style using a hidden Markov model.
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
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "set.h"
#include "vector.h"
#include "random.h"

using namespace std;

const string NUMBERS = "0123456789";

//Function prototypes:
void intro(void);
int getN(void);
ifstream getStream(void);
Map<Vector<string>, Vector<string>> createMap(ifstream &stream, int &n);
Vector<string> getInitialKey(ifstream &stream, int &n);
void buildKeyPair(string &word, Map<Vector<string>, Vector<string>> &map, Vector<string> &key);
void finishMap(Map<Vector<string>, Vector<string>> &map, Vector<string> &key, Vector<string> &initialKey);
int nWords(int &n);
void genRandomWords(Map<Vector<string>, Vector<string>> &map, int &nWords);
void printVector(Vector<string> &vector);

//main() function:
int main() {
    intro();
    Map<Vector<string>, Vector<string>> map;
    int runVar = 1;

    ifstream stream = getStream();
    int n = getN();

    //STEP 1: Build Map
    map = createMap(stream, n);

    while(runVar == 1){
        int numberOfWords = nWords(n);
        //(User enters 0 to quit)
        if(numberOfWords == 0){
            break;
        }
        //STEP 2: Generate Random Words
        genRandomWords(map, numberOfWords);
    }

    cout << "Exiting." << endl;
    return 0;
}


/* intro function:
 * Prints introductory statement and
 * keeps main() function clean.
 */
void intro(void) {
    cout << "Welcome to CS 106B Random Writer ('N-Grams'). " << endl;
    cout << "This program makes random text based on a document. " << endl;
    cout<< "Give me an input file and an 'N' value for groups " << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

/*getStream Function:
 *Returns stream object that is connected to the input text file.
 *This stream object is used to read in the input text file.
 */
ifstream getStream(void) {
    string fileName = getLine("Input file name? ");
    ifstream stream;
    while(!openFile(stream, fileName)) {
        cout << "Unable to open that file. Try again." << endl;
        fileName = getLine("Input file name? ");
    }
    return stream;
}

//getN Function:
//Return size of desired N-Gram
int getN(void) {
    int n;
    string input = (getLine("Value of N? "));
    int iteration = 0;
    for (char c : input){
        for(int i = 0; i < NUMBERS.size(); i++) {
            char num = NUMBERS[i];
            if(c == num){
                iteration++;
            }
        }
    }
    if(iteration == input.size()){
        n = stringToInteger(input);
    } else {
        cout << "Illegal Integer Format. Try again." << endl;
        return getN();
    }
    if(n == 0 || n == 1){
        cout << "N must be 2 or greater" << endl;
        return getN();
    } else {
        return n;
    }
}

//createMap Function:
//Creates map matching N-1 prefixes to suffixes.
Map<Vector<string>, Vector<string>> createMap(ifstream &stream, int &n) {
    string word;
    Map<Vector<string>, Vector<string>> map;
    Vector<string> initialKey;
    Vector<string> key;
    //Get initial key of length N-1:
    initialKey = getInitialKey(stream, n);
    key = initialKey;
    //Build entire map except for overlapping word pairs:
    while(stream >> word) {
        buildKeyPair(word, map, key);
    }
    //Adds overlapping word pairs to map:
    finishMap(map, key, initialKey);
    return map;
}

//getInitialKeyFunction:
//Returns initial N-1 key to start process of building map.
//This function is called in the createMap function.
Vector<string> getInitialKey(ifstream &stream, int &n) {
    string word;
    Vector<string> initialKey;
    for(int i = 1; i < n; i++) {
        stream >> word;
        initialKey.add(word);
    }
    return initialKey;
}

/*
 * buildKeyPair Function:
 *This function builds the key pairs, associating
 *each N-1 key with the word following the key.
 *If the key already exists, this function maps
 *adds the next word to the existing value vector. If the key does
 * not exist, the function creates a new key
 * and maps the next word to the new key.
 */
void buildKeyPair(string &word, Map<Vector<string>, Vector<string>> &map, Vector<string> &key) {
    //Step 1: Associate value with key
    if(map.containsKey(key)) {
        //If map already contains key, then add new value
        //to existing values associated with the key
        Vector<string> value = map.get(key);
        value.add(word);
        map.put(key, value);
    } else {
        Vector<string> value;
        value.add(word);
        map.put(key, value);
    }

    //Step 2: Create new key by removing old word and adding in new word:
    //Step 2, Part A: Add new word to end of key vector
    key.add(word);
    //Step 2, Part B: Remove oldest word from beginning of key vector:
    key.remove(0);
    return;
}

/* finishMap function:
* Finishes map by overlapping final N-1 words with initial N-1 words.
* For instance, if N = 3, the final N-1 words {the, house} are mapped
* to the first N-1 words {We, walk}, generating the new key pair
* {house, We}:{walk}.
*/
void finishMap(Map<Vector<string>, Vector<string>> &map, Vector<string> &key, Vector<string> &initialKey) {
    string word;
    int keySize = key.size();
    cout << endl;
    for(int i = 0; i < keySize; i++) {
        word = initialKey[i];
        buildKeyPair(word, map, key);
    }
    return;
}

/*nWords function:
 * Asks user how many words they would like to generate.
 * Handles case of non-integer input and case where
 * the number of words requested is less than N.
 * Returns desired number of words as integer.
 */
int nWords(int &n){
    int numberOfWords;
    string input = getLine("# of random words to generate (0 to quit)? ");
    int iteration = 0;
    for (char c : input){
        for(int i = 0; i < NUMBERS.size(); i++) {
            char num = NUMBERS[i];
            if(c == num){
                iteration++;
            }
        }
    }
    if(iteration == input.size()){
        numberOfWords = stringToInteger(input);
    } else {
        cout << "Illegal integer format. Try again." << endl;
        return nWords(n);
    }
    if(numberOfWords == 0) {
        return numberOfWords;
    }
    if(numberOfWords < n) {
        cout << "Must be at least " << n << " words" << endl;
        return nWords(n);
    } else {
        return numberOfWords;
    }
}

/* genRandomWords Function:
 * This function generates the number of random words
 * requested by the user using a hidden Markov model.
 * The function uses a map of N-1 prefixes with an associated vector
 * of suffixes. The function selects the next word by randomly choosing
 * a suffix with equal probability given to each suffix in the vector.
 * The function prints each word to the console and does
 * not return any objects
 */
void genRandomWords(Map<Vector<string>, Vector<string>> &map, int &numberOfWords){
    string nextWord;
    int valueLength;
    Vector<string> values;
    Vector<Vector<string>> keys = map.keys();
    int keySize = keys.size() - 1;
    int randInt = randomInteger(0, keySize);
    Vector<string> previousWords = keys[randInt];
    cout << endl;
    cout << "... ";
    printVector(previousWords);
    const int nMinusOne = previousWords.size();
    for(int i = 0; i < (numberOfWords - nMinusOne); i++) {
        values = map.get(previousWords);
        valueLength = values.size() - 1 ;
        //cout << "Values Size: " << values.size() << endl;
        randInt = randomInteger(0, valueLength);
        //cout << "Random Integer" << randInt << endl;
        nextWord = values[randInt];
        cout << nextWord << " ";
        //cout << "Next Word " << nextWord << endl;
        previousWords.remove(0);
        previousWords.add(nextWord);
        //cout << previousWords.toString() << endl;
    }
    cout << "..." << endl;

    return;
}

/* printVector function:
 * This function prints Vector<string> objects.
 * It is used in the genRandomWords function to
 * print the initial N-1 prefix vector
 */
void printVector(Vector<string> &vector){
    int vectorSize = vector.size();
    for(int i = 0; i < vectorSize; i++){
        //cout << vector[i] << " ";
        cout << vector[i] << " ";
    }
}
