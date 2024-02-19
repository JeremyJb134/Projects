/***********************************************************
 ** File:    Bar.cpp
 ** Project: CMSC 202 - Project 5
 ** Author:  Jeremy Bruce
 ** Date     5/2/2023
 ** Section: 11
 **********************************************************/


#include "Bar.h"

  // Name: Bar (Default Constructor) (not used)
  // Desc: Dynamically allocates m_data for strings (always). m_fileName defaults to test.txt
  // Preconditions: Input file has correctly formatted input
  // Postconditions: Returns list of items and their frequency (stored in LL)
Bar::Bar(){
    //ititalizes member variables
    m_fileName = "test.txt"; 
    m_data = new LL<string>();
}
  // Name: Bar (Overloaded Constructor)
  // Desc: Used to pass an input file
  //       Dynamically allocates m_data for strings (always)
  //       Sets m_fileName and nothing done with m_dataSorted until SortData
  // Preconditions: Input file
  // Postconditions: m_data is allocated, m_fileName populated
Bar::Bar(string filename){
    //itializes member variables with filename
    m_fileName = filename;
    m_data = new LL<string>();
}
  // Name: ~Bar (Destructor)
  // Desc: Deallocates m_data
  // Preconditions: m_data has been dynamically allocated
  // Postconditions: m_data is empty
Bar::~Bar (){
    delete m_data;
}
  // Name: Start
  // Desc: Calls LoadFile, RemoveCommon, RemoveSingles, Sort, DisplayBars, and Export
  //       During testing, may want to call Display
  // Preconditions: None (filename is available)
  // Postconditions: None
void Bar::Start(){
    LoadFile();
    string choice;
    cout << "Would you like to remove common words (Only use with essays)?" << endl;
    cin >> choice;
    if ((choice == "YES") or (choice == "Y") or (choice == "yes") or (choice == "y"))
        RemoveCommon();
    cout << "Would you like to remove all words with a frequency of 1?" << endl;
    cin >> choice;
    if ((choice == "YES") or (choice == "Y") or (choice == "yes") or (choice == "y"))
        RemoveSingles();
    SortData();
    DisplayBars();
    Export();
}
  // Name: LoadFile()
  // Desc: Loads a file (everything as a string although could be anything stringlike)
  //       Calls RemovePunct as each item is loaded
  //       Each input file is an unknown length
  // Preconditions: m_fileName is populated with good data
  // Postconditions: m_data is populated
void Bar::LoadFile(){
    ifstream inputStream;
    inputStream.open(m_fileName);
    string words;
    int counter = 0;
    //keeps reading file and putting it into words after every space or new line
    while(inputStream >> words){
        RemovePunct(words);
        m_data->Insert(words);
        counter++;
    }
    cout << counter << " loaded." << endl;
    inputStream.close();

}
  // Name: RemovePunct (passed a string)
  // Desc: Converts all input into lowercase using tolower
  //       When passed a string, removes all non letter punctation on the beginning
  //       or end of the string.
  //       These are all ok: "don't" "inner-circle" but "candy," is bad
  //       Is called during LoadFile
  //       You can use ispunct - https://www.geeksforgeeks.org/ispunct-function-c/
  // Preconditions: Strings to be evaluated
  // Postconditions: Punctuation is removed from string
void Bar::RemovePunct(string& data){
    //iteratres through whole string and makes character lower
    for(string::iterator it = data.begin(); it != data.end(); ++it){
        *it = tolower(*it);
    }
    //if punctuation at front, removes it at position 0
    if(ispunct(data.front()))
        data.erase(data.begin() + 0); 
    //if there is punctuation at the end, removes it at the size - 1
    if(ispunct(data.back()))
        data.erase(data.begin() + data.size() - 1);
    
}
  // Name: RemoveCommon
  // Desc: Asks user if they want to remove common words
  //       If yes, iterates through m_data and removes all common words after they are read
  // Preconditions: m_data is populated and EXCLUDE_LIST is populated
  // Postconditions: m_data will have no words from EXCLUDE_LIST
void Bar::RemoveCommon(){
    //if first is in the exclude list vector, remove it from linked list
    for (vector<string>::const_iterator it = EXCLUDE_LIST.begin(); it != EXCLUDE_LIST.end(); ++it){ //vector iterator
        if (m_data->Find(*it) != nullptr){ //uses find to see if its in the linked list
            m_data->RemoveAt(*it);
        }
    }
    cout << "All common words removed." << endl;
}
  // Name: RemoveSingles
  // Desc: Asks the user if they would like to remove words with a frequency of 1
  //       Should use the overloaded [] NOTE: Don't forget m_data is a pointer!
  // Preconditions: m_data is populated
  // Postconditions: m_data will have no words with a frequency of 1
void Bar::RemoveSingles(){
    int counter = 0;
    //iterates through the whole linked list
    for(int i = 0; i < m_data->GetSize(); i++){
        if((*m_data)[i].second == 1){ //removes from linked list if the second is 1
            m_data->RemoveAt((*m_data)[i].first);
            counter++;
            i--; //decreases i by one to not go out of scope
        }
    }
    cout << counter << " words removed." << endl;
}
  // Name: SortData
  // Desc: Iterates through each items in m_data and inserts into
  //       m_dataSorted using the frequency as the key (can use make_pair)
  // Preconditions: m_data is populated
  // Postconditions: m_dataSorted is populated and sorted based on frequency
void Bar::SortData(){
    for(int i = 0; i < m_data->GetSize(); i++){
        //loops through m_data and inserts into dataSorted, and switched first and second
        m_dataSorted.insert(make_pair((*m_data)[i].second, (*m_data)[i].first));
    }
}
  // Name: DisplayBars
  // Desc: Displays a bar chart for each item in m_dataSorted from highest
  //       frequency (at the top) to lowest frequency (at the bottom) (iterate backwards)
  // Preconditions:  m_dataSorted is populated
  // Postconditions: Bar charts are displayed
void Bar::DisplayBars(){
    //sets const for formatting
    const int format = 15;
    //starts from the end of the list using reverse iterator and iterates through
    for(multimap<int,string>::reverse_iterator it = m_dataSorted.rbegin(); it != m_dataSorted.rend(); ++it){
        //sets witdh and outputs the word
        cout << setw(format) << it->second << ": ";
        //couts * based on first number 
        for(int i = 0; i < it->first; i++){
            cout << barChar;
        }
        cout << endl;
    }
}
  // Name: Export
  // Desc: Asks the user what name the export file should be called
  //       Iterates through m_dataSorted and exports to file
  // Preconditions: m_dataSorted is populated
  // Postconditions: New export file is created and populated with a formatted bar chart
void Bar::Export(){
    //sets const for formatting
    const int format = 15;
    ofstream outputStream;
    string outfile;
    //asks the user to give file name and opens the file based on name
    cout << "What would you like to call the export file?" << endl;
    cin >> outfile;
    outputStream.open(outfile);
    for(multimap<int,string>::reverse_iterator it = m_dataSorted.rbegin(); it != m_dataSorted.rend(); ++it){
    //sets witdh and outputs the word
        outputStream << setw(format) << it->second << ": ";
    //couts * based on first number 
        for(int i = 0; i < it->first; i++){
            outputStream << barChar;
        }
    outputStream << endl;
    }
    cout << "Output to exportFile " << outfile << endl;
  }