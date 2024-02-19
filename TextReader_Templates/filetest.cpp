#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

int main(){ 
    ifstream inputStream;
    inputStream.open("proj5_test3.txt");
    string words;
    while(inputStream >> words){
        for(string::iterator it = words.begin(); it != words.end(); ++it){
            *it = tolower(*it);
        }
        if(ispunct(words.front()))
            words.erase(words.begin() + 0);
        if (words.back() == '\n')
            words.pop_back();
        if(ispunct(words.back()))
            words.erase(words.begin() + words.size() - 1);
    
    cout << words << endl;
    }
    
}