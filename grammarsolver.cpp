/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Sarthak Tandon
 * Section leader: Norah Borus
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"
#include "map.h"
#include "strlib.h"
using namespace std;

int isTerminal(vector<string> nonTerminals, string s);
void createSentence(string&, string, map<string, vector<string>>, vector<string>);

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */

Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    string line;
    vector<string> nonTerminals, splitLine;
    map<string, vector<string>> map;

    //Adding first word of each line to vector "nonTerminals".
    while(!input.eof()){
        getline(input, line);
        splitLine = stringSplit(line, ":");
        string currentNonTerminal = splitLine[0];
        nonTerminals.insert(nonTerminals.end(), currentNonTerminal);
        splitLine.clear();
    }

    //Resetting get pointer.
    input.seekg(0);

    //Mapping all non-terminals to their respective right sides while splitting the right side by "|".
    int i = 0;
    while(!input.eof()){
        getline(input, line);
        splitLine = stringSplit(line, "=");
        string rightSide = splitLine[1];
        trim(rightSide);
        vector<string> connections = stringSplit(rightSide, "|");
        map[nonTerminals[i++]]= connections;
    }

    //Adding sentences one by one to the vector allSentences.
    string currentSentence;
    vector<string> allSentences;
    for (int i =0; i<times; i++){
        createSentence(currentSentence, symbol, map, nonTerminals); //Recursive Function to create string "currentSentence".
        allSentences.insert(allSentences.end(), currentSentence);
        currentSentence.clear();
    }

    return allSentences;

}

void createSentence(string &sentence, string symbol, map<string, vector<string>> map, vector<string> nonTerminals){

    //Base Case (If symbol is a terminal, just append it to the sentence).
    if (isTerminal(nonTerminals, symbol)){
        sentence.append(symbol);
        sentence.append(" ");
    }
    // Recursive Case(If symbol is not a terminal, use map to change symbol and recurse).
    else {
        vector<string> connections = map[symbol];  //Maps the current symbol to its corr. vector of connections.
        int i = randomInteger(0, connections.size()-1); //Generates random integer to select a connection.
        string mappedString = connections[i];
        trim(mappedString);
        vector<string> splitMappedString = stringSplit(mappedString," ");
        i = 0;
        while(i<splitMappedString.size()){  //Traverses each letter of that connection string.
            createSentence(sentence, splitMappedString[i], map, nonTerminals);  //recurses with new symbol.
            i++;
        }
    }
}

int isTerminal(vector<string> nonTerminals, string s){
    for (int i=0; i<nonTerminals.size(); i++){  //Loops over all non-terminals and compares the symbol "s".
        if (s == nonTerminals[i]){
            return 0;
        }
    }
    return 1;
}
