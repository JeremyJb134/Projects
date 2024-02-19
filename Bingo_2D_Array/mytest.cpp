// mytest.cpp
// Jeremy Bruce
// 09/11/2023

#include "bingo.h"
class Tester{
    public:
        bool constructSizeError(){
            //tests to see if object is empty with invalid columns and rows
            cout << "Creating object with -5 rows and -10 columns" << endl;
            Bingo test(-5, -10, 11, 85);
            if (test.m_numRows == 0 and test.m_numCols == 0){
                return true;
            }
            return false;
        }
        bool constructRangeError(){
            //tests to see if an object is empty with given range not divisible by 5
            cout << "Creating object with range not divisible by 5" << endl;
            Bingo test(5,5, 10, 85);
            if (test.m_maxBallVal == 0 and test.m_minBallVal == 0){
                return true;
            }
            return false;
        }
        bool constructCorrect(){
            //test to see if an object with valid rows, columns, and range is created
            cout << "Creating object with 5 rows, 5 columns, and correct range" << endl;
            Bingo test(5,5,11,85);
            //tests to see each member variable is correctly intialized
            if(test.m_maxBallVal == 85 and test.m_minBallVal == 11 and test.m_helper != nullptr and test.m_trackCols != nullptr
            and test.m_trackRows != nullptr and test.m_card != nullptr and test.m_numRows == 5 and test.m_numCols == 5){
                cout << "Object created with 5 rows, 5 columns, and correct range" << endl << endl;
            }
            else{
                cout << "Object not created with correct values" << endl << endl;
            }
            //tests for edge cases
            Bingo test2(2,5,11,85);
            cout << "Creating object with 2 rows, which is lowest possible rows" << endl;
            if(test2.m_maxBallVal == 85 and test2.m_minBallVal == 11 and test2.m_helper != nullptr and test2.m_trackCols != nullptr
            and test2.m_trackRows != nullptr and test2.m_card != nullptr and test2.m_numRows == 2 and test2.m_numCols == 5){
                cout << "Object created with 2 rows, 5 columns, and correct range" << endl << endl;
            }
            else{
                cout << "Object not created with correct values" << endl << endl;
            }
            Bingo test3(15,5,11,85);
            cout << "Creating object with 15 rows, which is highest possible rows" << endl;
            if(test3.m_maxBallVal == 85 and test3.m_minBallVal == 11 and test3.m_helper != nullptr and test3.m_trackCols != nullptr
            and test3.m_trackRows != nullptr and test3.m_card != nullptr and test3.m_numRows == 15 and test3.m_numCols == 5){
                return true;
            }
            else{
                return false;
            }
        }
        bool reCreateError(){
            //tests reCreateCard function
            Bingo test(5,5,11,85);
            //initializes card so we can recreate
            test.initCard();
            cout << "Recreating object with rows -5 and columns -10" << endl;
            test.clear();
            //tests to see if recreates an empty object if invalid rows and columns
            if(!(test.reCreateCard(-5,-10,11,85))){
                cout << "Empty object created after recreating" << endl << endl;
            }
            else{
                cout << "Empty object not created" << endl << endl;
            }
            //tests to see if recreates an object with valid rows and columns
            test.clear();
            cout << "Recreating object with 2 rows and 5 columns" << endl;
            if(test.reCreateCard(2,5,11,85)){
                //makes sure each value is successfuly assigned
                if(test.m_maxBallVal == 85 and test.m_minBallVal == 11 and test.m_helper != nullptr and test.m_trackCols != nullptr
                and test.m_trackRows != nullptr and test.m_card != nullptr and test.m_numRows == 2 and test.m_numCols == 5){
                    cout << "Object successfully recreated with 2 rows, 5 columns, and correct range" << endl << endl;
                }
                else{
                    cout << "Recreate was not successful" << endl << endl;
                }
            }
            test.clear();
            //tests max edge case
            cout << "Recreating object with 15 rows and 5 columns" << endl;
            if(test.reCreateCard(15,5,11,85)){
                if(test.m_maxBallVal == 85 and test.m_minBallVal == 11 and test.m_helper != nullptr and test.m_trackCols != nullptr
                and test.m_trackRows != nullptr and test.m_card != nullptr and test.m_numRows == 15 and test.m_numCols == 5){
                    cout << "Object successfully recreated with 15 rows, 5 columns, and correct range" << endl << endl;
                }
                else{
                    cout << "Recreate was not successful" << endl << endl;
                }
            }
            test.clear();
            cout << "Recreating object with 5 rows and 5 columns" << endl;
            if(test.reCreateCard(5,5,11,85)){
                if(test.m_maxBallVal == 85 and test.m_minBallVal == 11 and test.m_helper != nullptr and test.m_trackCols != nullptr
                and test.m_trackRows != nullptr and test.m_card != nullptr and test.m_numRows == 5 and test.m_numCols == 5){
                    return true;
                }
            }
            return false;
        }
        bool initCardTest(){
            Bingo test(5,5,11,85);
            cout << "Testing initializing card on non empty object" << endl;
            //tests to intialize card on a valid object
            int set = (85 - 11 + 1)/5; //calculates how much is distributed to each column
            if(test.initCard()){
                cout << "Initialized object successfully created" << endl;
                bool flag = true;
                //tests to see if each column contains the correct set of numbers
                //makes flag false if number is less than the min for the column or greater then the max of the column
                for(int i = 0; i < test.m_helperSize; i++){
                    if(test.m_helper[i].getCol() == 0 and test.m_helper[i].getVal() < 11
                    and test.m_helper[i].getVal() >= (set + 11)) 
                        flag = false; 
                    if(test.m_helper[i].getCol() == 1 and test.m_helper[i].getVal() < (11 + set)
                    and test.m_helper[i].getVal() >= (2 * set + 11))
                        flag = false;
                    if(test.m_helper[i].getCol() == 2 and test.m_helper[i].getVal() < (11 + 2 * set)
                    and test.m_helper[i].getVal() >= (3 * set + 11))
                        flag = false;
                    if(test.m_helper[i].getCol() == 3 and test.m_helper[i].getVal() < (11 + 3 * set)
                    and test.m_helper[i].getVal() >= (4 * set + 11))
                        flag = false;
                    if(test.m_helper[i].getCol() == 4 and test.m_helper[i].getVal() < (11 + 4 * set)
                    and test.m_helper[i].getVal() >= (5 * set + 11))
                        flag = false;
                }
                if (flag == true)
                    cout << "Numbers in each column distributed correctly" << endl << endl;
                else
                    cout << "Numbers are not distributed in correct columns." << endl << endl;
            }
            else{
                cout << "Card not intialized" << endl << endl;
            }
            test.clear();
            //tests to see if intialize card returns false for an empty object
            cout << "Testing intializing card on empty object" << endl;
            if(!(test.initCard())){
                return true;
            }
            return false;
        }
        bool playError(){
            Bingo test(5,5,11,85);
            test.initCard();
            vector<int> testVector(86);
            int draws = 80;
            cout << "Testing to play with a vector greater than max number of balls" << endl;
            //if returns 0, then no balls were drawn
            if(test.play(draws, testVector) == -1)
                cout << "Successful, no balls were drawn" << endl << endl;
            else
                cout << "Error, balls were drawn" << endl << endl;
            draws = 86;
            cout << "Testing to play with number of draws greater then max number of balls" << endl;
            //if returns 0, then no balls were drawn
            if(test.play(draws, test.drawBalls()) == -1)
                cout << "Successful, no balls were drawn" << endl << endl;
            else
                cout << "Error, balls were draw" << endl << endl;
            draws = 75;
            cout << "Testing to play with draws equal to max number of balls" << endl;
            //if number of balls drawn are equal to max ball value, then game ends when one row or column is completed
            //the number of replacements should be less than the number of cells in the card
            if(test.play(draws, test.drawBalls()) < (test.m_numRows * test.m_numCols)){
                bool flag = false;
                //sets flag to true if either a row or column is completed
                for (int i = 0; i < test.m_numRows; i++){
                    if(test.m_trackRows[i] == test.m_numCols)
                        flag = true;
                }
                for (int i = 0; i < test.m_numCols; i++){
                    if(test.m_trackCols[i] == test.m_numRows)
                        flag = true;
                }
                if (flag == true){
                    cout << "Successful, balls were drawn until one row was completed" << endl << endl;
                }
                else{
                    cout << "Error, game ended without a row or column completed" << endl << endl;
                }
            }
            else{
                cout << "Error, game did not end after a row or column was completed" << endl << endl;
            }
            test.clear();
            test.reCreateCard(5,5,11,85);
            cout << "Testing to play with draws less than rows and columns, so no row or column completed" << endl;
            //if number of draws does not equal max ball value, then game ends when the number of drawings is reached
            test.play(4, test.drawBalls());
            bool flag = true;
            //checks to make sure the game did not end until a row or column was completed
            for (int i = 0; i < test.m_numRows; i++){
                if(test.m_trackRows[i] == test.m_numCols)
                    flag = false;
            }
            for (int i = 0; i < test.m_numCols; i++){
                if(test.m_trackCols[i] == test.m_numRows)
                    flag = false;
            }
            if(flag == true){
                cout << "Successful, game did not continue until a row or column is completed" << endl << endl;
            }
            else
                cout << "Error, game continued until a row or column was completed" << endl << endl;
            test.clear();
            test.reCreateCard(5,5,11,85);
            cout << "Testing to play with a large amount of draws, so game continues after row or column is completed" << endl;
            //the card is supposed to be mostly ot fully filled after the game since there are only 75 numbers that can be drawn
            flag = true;
            test.play(70, test.drawBalls());
            //tests to see if every row and column is filled
            for (int i = 0; i < test.m_numRows; i++){
                if(test.m_trackRows[i] != test.m_numCols)
                    flag = false;
            }
            for (int i = 0; i < test.m_numCols; i++){
                if(test.m_trackCols[i] != test.m_numRows)
                    flag = false;
            }
            return flag;
        }
        // This function is a sample test function
        // It shows how to write a test case 
        bool assignmentNormal(Bingo & lhs, Bingo & rhs){
            // we expect that lhs object is an exact copy of rhs object 
            bool result = true;
            // we expect that the corresponding cells in lhs and rhs
            //      cards carry the same cell information (exact same copy)
            for (int i=0;i<rhs.m_numRows;i++){
                for (int j=0;j<rhs.m_numCols;j++){
                    result = result && (lhs.m_card[i][j] == rhs.m_card[i][j]);
                }
            }
            // we expect that the corresponding cells in lhs and rhs
            //      m_helper carry the same cell information (exact same copy)
            for (int i=0;i<rhs.m_helperSize;i++){
                result = result && (lhs.m_helper[i] == rhs.m_helper[i]);
            }
            // we expect that the corresponding cells in lhs and rhs
            //      m_trackRows carry the same cell information (exact same copy)
            for (int i=0;i<rhs.m_numRows;i++){
                result = result && (lhs.m_trackRows[i] == rhs.m_trackRows[i]);
            }
            // we expect that the corresponding cells in lhs and rhs
            //      m_trackCols carry the same cell information (exact same copy)
            for (int i=0;i<rhs.m_numCols;i++){
                result = result && (lhs.m_trackCols[i] == rhs.m_trackCols[i]);
            }
            result = result && (lhs.m_minBallVal == rhs.m_minBallVal);
            result = result && (lhs.m_maxBallVal == rhs.m_maxBallVal);
            return result;
        }
};

int main(){
    Tester tester;
    if(tester.constructSizeError()){
        cout << "Empty object created" << endl << endl;
    }
    else{
        cout << "Error, object not empty" << endl << endl;
    }
    if(tester.constructRangeError()){
        cout << "Empty object created" << endl << endl;
    }
    else{
        cout << "Error, object not empty" << endl << endl;
    }
    if(tester.constructCorrect()){
        cout << "Object successfully created with 15 rows" << endl << endl;
    }
    else{
        cout << "Object not created with correct values" << endl << endl;
    }
    if(tester.reCreateError()){
        cout << "Recreated card successful" << endl << endl;
    }
    else{
        cout << "Recreated card was not successful" << endl << endl;
    }
    if(tester.initCardTest()){
        cout << "Successful, object still empty" << endl << endl;
    }
    else{
        cout << "Error, object not empty" << endl << endl;
    }
    if(tester.playError()){
        cout << "Successful, game ended after 75 draws" << endl << endl;
    }
    else{
        cout << "Error, game ended when a row or column was completed" << endl << endl;
    }
    Bingo test1(5,5,11,85);
    test1.initCard();
    Bingo test2;
    cout << "Testing assignment operator" << endl;
    test2 = test1;
    if(tester.assignmentNormal(test1, test2)){
        cout << "Assignment operator successfully worked" << endl << endl;
    }
    else{
        cout << "Assignment operator failed" << endl << endl;
    }
    test1.clear();
    test2 = test1;
    cout << "Testing to assign an empty object onto a non empty object" << endl;
    if(tester.assignmentNormal(test1, test2)){
        cout << "Assignment operator successfully worked" << endl << endl;
    }
    else{
        cout << "Assignment operator failed" << endl << endl;
    }
    return 0;
}