// UMBC - CMSC 341 - Fall 2023 - Proj0
#include "bingo.h"
class Tester{
    public:
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
    private:
    
    /**********************************************
    * If we need helper functions to be reused in *
    *   test functions they can be declared here!
    **********************************************/
};
int main(){
    Tester tester;
    Bingo obj1(CARDROWS,CARDCOLS,MINVAL,MAXVAL);
    vector<int> balls = obj1.drawBalls();
    if (obj1.initCard()){
        obj1.dumpCard();
        cout << "\ngame over after " << obj1.play(MAXVAL,balls) << " hits!\n" << endl;
        obj1.dumpCard();
    }
    Bingo obj2;
    obj2 = obj1;
    // test whether the assignment operator normal case works correctly
    cout << "Following is the test message for testing assignment operator:\n" << endl;
    if (tester.assignmentNormal(obj2,obj1))
        cout << "Test msg: Assignment operator test for normal case passed!\n" << endl;
    else
        cout << "Test msg: Assignment operator test for normal case failed!\n" << endl;

    // Re-initialize the object to a card of 10x5
    cout << "Following is the result of re-initializing the object and re-play:\n" << endl;
    //obj2.clear();
    //obj2.dumpCard();
    //cout << "obj2 cleared" << endl;
    obj1.clear();
    if (obj1.reCreateCard(2*CARDROWS,CARDCOLS,MINVAL,MAXVAL)){
        obj1.initCard();
        obj1.dumpCard();
        balls = obj1.drawBalls();
        cout << "\ngame over after " << obj1.play(MAXVAL,balls) << " hits!\n" << endl;
        obj1.dumpCard();
    }
    
    cout << "The following is the dump of object created by overloaded assignment operator:\n" << endl;
    obj2.dumpCard();
    
    return 0;
}