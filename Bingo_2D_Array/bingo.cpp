// UMBC - CMSC 341 - Fall 2023 - Proj0
// bingo.cpp
// Jeremy Bruce
// 9/11/2023
#include "bingo.h"
Bingo::Bingo(){
    //sets all ints to 0 and all pointers to nullptr
    m_numRows = 0;
    m_numCols = 0;
    m_minBallVal = 0;
    m_maxBallVal = 0;
    m_trackCols = nullptr;
    m_trackRows = nullptr;
    m_helper = nullptr;
    m_helperSize = 0;
    m_card = nullptr;
}

Bingo::Bingo(int rows, int columns, int min, int max){
    const int rowMin = 2;
    const int rowMax = 15;
    const int colLength = 5;
    //checks to make sure valid rows, columns, min, and max are given
    //if max - min + 1 % 5 does not equal 0, then the numbers can't be evenly distributed between columns
    //also makes sure there are enough numbers to fill in the entire card
    if ((rows >= rowMin) and (rows <= rowMax) and (columns == colLength) and ((max - min + 1) % colLength == 0) 
    and ((max - min + 1) >= (rows * columns))){
        m_numRows = rows;
        m_numCols = columns;
        m_minBallVal = min;
        m_maxBallVal = max;
        //initializes all dynamic arrays
        m_trackCols = new int[columns];
        m_trackRows = new int[rows];
        m_helperSize = rows * columns;
        m_helper = new Cell[m_helperSize];
        //initializes each array at each row in the 2D array
        m_card = new Cell*[rows];
        for (int i = 0; i < rows; i++){
            m_card[i] = new Cell[columns];
        } 
    }
    else{
        //sets everything to 0 or nullptr if conditions are not met
        m_numRows = 0;
        m_numCols = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_trackCols = nullptr;
        m_trackRows = nullptr;
        m_helper = nullptr;
        m_helperSize = 0;
        m_card = nullptr;
    }
    
}

bool Bingo::reCreateCard(int rows, int columns, int min, int max){
    const int rowMin = 2;
    const int rowMax = 15;
    const int colLength = 5;
    //same as constructor, checks each value and then initalizes and allocates memory
    if ((rows >= rowMin) and (rows <= rowMax) and (columns == colLength) and ((max - min + 1) % colLength == 0)
    and ((max - min + 1) >= (rows * columns))){
        m_numRows = rows;
        m_numCols = columns;
        m_minBallVal = min;
        m_maxBallVal = max;
        m_trackCols = new int[columns];
        m_trackRows = new int[rows];
        m_helperSize = rows * columns;
        m_helper = new Cell[m_helperSize];
        m_card = new Cell*[rows];
        for (int i = 0; i < rows; i++){
            m_card[i] = new Cell[columns];
        } 
        return initCard(); //calls initialize card function since it also returns bool
    }
    else{
        //creates empty object if condtions aren't met
        m_numRows = 0;
        m_numCols = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_trackCols = nullptr;
        m_trackRows = nullptr;
        m_helper = nullptr;
        m_helperSize = 0;
        m_card = nullptr;
    }
    return false;
    
}

Bingo::~Bingo(){
    //deletes each array if it doesn't equal nullptr, so memory is allocated
    if (m_trackCols != nullptr){
        delete[] m_trackCols;
        m_trackCols = nullptr;
    }
    if (m_trackRows != nullptr){
        delete[] m_trackRows;
        m_trackRows = nullptr;

    }
    if (m_helper != nullptr){
        delete[] m_helper;
        m_helper = nullptr;
    }
    m_helperSize = 0;
    if (m_card != nullptr){
        for (int i = 0; i < m_numRows; i++){
            if (m_card[i] != nullptr){
                //deletes each array at each row in 2D array
                delete[] m_card[i];
                m_card[i] = nullptr;
            }
                
        }
        delete[] m_card; //deletes m_card at end
        m_card = nullptr;
    }
    m_numRows = 0;
    m_numCols = 0;
    m_minBallVal = 0;
    m_maxBallVal = 0;
}

void Bingo::clear(){
    //same a destructor, deletes each array if memory is allocated
    if (m_trackCols != nullptr){
        delete[] m_trackCols;
        m_trackCols = nullptr;
    }
    if (m_trackRows != nullptr){
        delete[] m_trackRows;
        m_trackRows = nullptr;
    }
    if (m_helper != nullptr){
        delete[] m_helper;
        m_helper = nullptr;
    }
    m_helperSize = 0;
    if (m_card != nullptr){
        for (int i = 0; i < m_numRows; i++){
            if (m_card[i] != nullptr){ //deletes each array at each row of 2D array
                delete[] m_card[i];
                m_card[i] = nullptr;
            }
        }
        delete[] m_card;
        m_card = nullptr;
    }
    m_numRows = 0;
    m_numCols = 0;
    m_minBallVal = 0;
    m_maxBallVal = 0;
}

bool Bingo::initCard(){
    //checks to make sure there is memory allocated for each array
    if(m_numCols != 0 and m_numRows != 0){ 
        for (int i = 0; i < m_numRows; i++){
            m_trackRows[i] = 0; //sets each index to 0
        }   
        for (int i = 0; i < m_numCols; i++){
            m_trackCols[i] = 0; //sets each index to 0
        }
        int counter = 0; //keeps track of index in m_helper
        int set = (m_maxBallVal - m_minBallVal + 1) / 5; //calculates the distribution between each column
        //nested for loop to loop through 2D array
        for (int i = 0; i < m_numCols; i++){
            int min = m_minBallVal + i * set; //calculates the min for each column
            int max = m_minBallVal + (i+1) * set - 1; //calculates the max for each column
            Random randNum(min, max); //creates Random object using min and max
            vector<int> numList;
            for(int k = 0; k < m_numRows; k++){
                //gets enough numbers to fill an entire column, and stores it in the vector
                numList.push_back(randNum.getRandNum());
            }
            for (int j = 0; j < m_numRows; j++){
                //sets each index for row and column of both m_card and m_helper
                m_card[j][i].setRow(j);
                m_card[j][i].setCol(i);
                m_helper[counter].setRow(j);
                m_helper[counter].setCol(i);
                //pulls a number from the vector holding the random numbers and sets it as the value
                int newNum = numList.at(j);
                m_card[j][i].setVal(newNum);
                m_helper[counter].setVal(newNum);
                counter++;
            }
        }   
        return true;
    }
    else{ //returns false if there is no memory allocated
        return false;
    }
}

vector<int> Bingo::drawBalls(){
    vector<int> balls;
    //takes the min ball val and max ball val, and shuffles the numbers onto a vector
    Random randVect(m_minBallVal, m_maxBallVal);
    randVect.getShuffle(balls);
    return balls;
}

int Bingo::play(int numDraws, vector<int> rndBalls){
    int tracker = 0; //tracks the number of replacements made
    //if the size of vector is too big or the numDraws is greater then max ball value, no balls are drawn
    if(numDraws > m_maxBallVal or int(rndBalls.size()) > m_maxBallVal or int(rndBalls.size()) == 0){
        return -1;
    }
    //if numbDraws equals maxBallVall, then game ends when a row or column is completed
    else if(numDraws == m_maxBallVal){
        for(unsigned int i = 0; i < rndBalls.size(); i++){ //loops through ball vector
            for(int j = 0; j < m_helperSize; j++){
                //goes through each cell in m_helper and check if it equals the ball at each index of the ball vector
                if (m_helper[j].getVal() == rndBalls.at(i)){
                    tracker++; 
                    m_helper[j].setVal(EMPTYCELL); 
                    m_card[m_helper[j].getRow()][m_helper[j].getCol()].setVal(EMPTYCELL);
                    //increments both track rows and columns using m_helper
                    m_trackRows[m_helper[j].getRow()]++;
                    m_trackCols[m_helper[j].getCol()]++;
                    //returns tracker if a row or column is completed
                    //if number of hits on a row = number of columns, then that row is completed
                    //if number of hits on a column = number of rows, then that column is completed
                    if (m_trackRows[m_helper[j].getRow()] == m_numCols or m_trackCols[m_helper[j].getCol()] == m_numRows)
                        return tracker;
                }
            }
        }
    }
    else{
        //loops using the numDraws
        for(int i = 0; i < numDraws; i++){
            for(int j = 0; j < m_helperSize; j++){
                if (m_helper[j].getVal() == rndBalls.at(i)){
                    tracker++;
                    m_helper[j].setVal(EMPTYCELL);
                    m_card[m_helper[j].getRow()][m_helper[j].getCol()].setVal(EMPTYCELL);
                    m_trackRows[m_helper[j].getRow()]++;
                    m_trackCols[m_helper[j].getCol()]++;
                }
            }
        }
        return tracker; //returns tracker after the number of draws is finished
    }
    return 0;
}

const Bingo & Bingo::operator=(const Bingo & rhs){
    if(this != &rhs){ //makes sure its not self assignment
        clear(); //clears the card to recreate it
        //sets each member variable to equal rhs member variable
        m_numRows = rhs.m_numRows;
        m_numCols = rhs.m_numCols;
        m_helperSize = rhs.m_helperSize;
        m_maxBallVal = rhs.m_maxBallVal;
        m_minBallVal = rhs.m_minBallVal;
        //if rhs arrays does not equal nullptr, allocate memory for an array
        //sets each index of the array equal to each other
        if(rhs.m_trackCols != nullptr){
            m_trackCols = new int[m_numCols];
            for (int i = 0; i < m_numCols; i++){
                m_trackCols[i] = rhs.m_trackCols[i];
            }
        }
        if (rhs.m_trackRows != nullptr){
            m_trackRows = new int[m_numRows];
            for(int i = 0; i < m_numRows; i++){
                m_trackRows[i] = rhs.m_trackRows[i];
            }
        }
        if (rhs.m_helper != nullptr){
            //allocates memory for both m_helper and m_card
            m_helper = new Cell[m_helperSize];
            m_card = new Cell*[m_numRows];
            for(int j = 0; j < m_numRows; j++){
                //allocates memory for each column
                m_card[j] = new Cell[m_numCols];
            }
            for (int i = 0; i < m_helperSize; i++){
                //first sets m_helper col,row, and val to equal rhs m_helper
                m_helper[i].setCol(rhs.m_helper[i].getCol());
                m_helper[i].setRow(rhs.m_helper[i].getRow());
                m_helper[i].setVal(rhs.m_helper[i].getVal());
                //uses m_helper to set each index of the 2D array
                m_card[m_helper[i].getRow()][m_helper[i].getCol()].setVal(m_helper[i].getVal());
                m_card[m_helper[i].getRow()][m_helper[i].getCol()].setRow(m_helper[i].getRow());
                m_card[m_helper[i].getRow()][m_helper[i].getCol()].setCol(m_helper[i].getCol());
            }
        }
    }
    return *this;
}

// The dump function renders the card in the terminal
// This function is provided to facilitate debugging
// Using this function as a test case will not be accepted
void Bingo::dumpCard(){
    cout << "  ";
    cout << "\033[1;35m B   I   N   G   O\033[0m";    
    cout << endl;
    for (int i=1;i <= m_numRows;i++){
        if ((i < 10))
            cout << "\033[1;35m" << "0" << i << " \033[0m";
        else
            cout << "\033[1;35m" << i << " \033[0m";
        for (int j=1;j<=m_numCols;j++){
            if (m_card[i-1][j-1].getVal() == EMPTYCELL)
                cout << "\033[1;31m" << m_card[i-1][j-1].getVal() << "\033[0m" << "  ";
            else
                cout << m_card[i-1][j-1].getVal() << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

// the overloaded equality operator for the Cell object
bool operator==(const Cell & lhs, const Cell & rhs){
    return ((lhs.m_col == rhs.m_col) &&
            (lhs.m_row == rhs.m_row) &&
            (lhs.m_value == rhs.m_value));
}
