#ifndef BAR_H
#define BAR_H

#include "LL.cpp"
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

//DO NOT ADD ANY ADDITIONAL CONSTANTS OR FUNCTIONS TO ANY CLASS

//CONSTANTS
const char barChar = '*';

//Common words to be removed from output
const vector<string> EXCLUDE_LIST {
  "a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", "can", "can't", "cannot", "com", "could", "couldn't", "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down", "during", "each", "else", "ever", "few", "for", "from", "further", "get", "had", "hadn't", "has", "hasn't", "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's", "http", "i", "i'd", "i'll", "i'm", "i've", "I", "I'd", "I'll", "I'm", "I've", "if", "in", "into", "is", "isn't", "it", "it's", "its", "itself", "just", "k", "let's", "like", "me", "more", "most", "mustn't", "my", "myself", "no", "nor", "not", "of", "off", "on", "once", "only", "or", "other", "ought", "our", "ours", "ourselves", "out", "over", "own", "r", "same", "shall", "shan't", "she", "she'd", "she'll", "she's", "should", "shouldn't", "so", "some", "such", "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then", "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've", "this", "those", "through", "to", "too", "under", "until", "up", "very", "was", "wasn't", "we", "we'd", "we'll", "we're", "we've", "were", "weren't", "what", "what's", "when", "when's", "where", "where's", "which", "while", "who", "who's", "whom", "why", "why's", "with", "won't", "would", "wouldn't", "www", "you", "you'd", "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"
};

class Bar{
 public:
  // Name: Bar (Default Constructor) (not used)
  // Desc: Dynamically allocates m_data for strings (always). m_fileName defaults to test.txt
  // Preconditions: Input file has correctly formatted input
  // Postconditions: Returns list of items and their frequency (stored in LL)
  Bar();
  // Name: Bar (Overloaded Constructor)
  // Desc: Used to pass an input file
  //       Dynamically allocates m_data for strings (always)
  //       Sets m_fileName and nothing done with m_dataSorted until SortData
  // Preconditions: Input file
  // Postconditions: m_data is allocated, m_fileName populated
  Bar(string filename);
  // Name: ~Bar (Destructor)
  // Desc: Deallocates m_data
  // Preconditions: m_data has been dynamically allocated
  // Postconditions: m_data is empty
  ~Bar ();
  // Name: Start
  // Desc: Calls LoadFile, RemoveCommon, RemoveSingles, Sort, DisplayBars, and Export
  //       During testing, may want to call Display
  // Preconditions: None (filename is available)
  // Postconditions: None
  void Start();
  // Name: LoadFile()
  // Desc: Loads a file (everything as a string although could be anything stringlike)
  //       Calls RemovePunct as each item is loaded
  //       Each input file is an unknown length
  // Preconditions: m_fileName is populated with good data
  // Postconditions: m_data is populated
  void LoadFile();
  // Name: RemovePunct (passed a string)
  // Desc: Converts all input into lowercase using tolower
  //       When passed a string, removes all non letter punctation on the beginning
  //       or end of the string.
  //       These are all ok: "don't" "inner-circle" but "candy," is bad
  //       Is called during LoadFile
  //       You can use ispunct - https://www.geeksforgeeks.org/ispunct-function-c/
  // Preconditions: Strings to be evaluated
  // Postconditions: Punctuation is removed from string
  void RemovePunct(string&);
  // Name: RemoveCommon
  // Desc: Asks user if they want to remove common words
  //       If yes, iterates through m_data and removes all common words after they are read
  // Preconditions: m_data is populated and EXCLUDE_LIST is populated
  // Postconditions: m_data will have no words from EXCLUDE_LIST
  void RemoveCommon();
  // Name: RemoveSingles
  // Desc: Asks the user if they would like to remove words with a frequency of 1
  //       Should use the overloaded [] NOTE: Don't forget m_data is a pointer!
  // Preconditions: m_data is populated
  // Postconditions: m_data will have no words with a frequency of 1
  void RemoveSingles();
  // Name: SortData
  // Desc: Iterates through each items in m_data and inserts into
  //       m_dataSorted using the frequency as the key (can use make_pair)
  // Preconditions: m_data is populated
  // Postconditions: m_dataSorted is populated and sorted based on frequency
  void SortData();
  // Name: DisplayBars
  // Desc: Displays a bar chart for each item in m_dataSorted from highest
  //       frequency (at the top) to lowest frequency (at the bottom) (iterate backwards)
  // Preconditions:  m_dataSorted is populated
  // Postconditions: Bar charts are displayed
  void DisplayBars();
  // Name: Export
  // Desc: Asks the user what name the export file should be called
  //       Iterates through m_dataSorted and exports to file
  // Preconditions: m_dataSorted is populated
  // Postconditions: New export file is created and populated with a formatted bar chart
  void Export();
 private: 
  string m_fileName; //Name of the input file
  LL<string>* m_data; //LL to store all data
  multimap<int, string> m_dataSorted; //multimap to sort the data based on frequency
};

#endif
