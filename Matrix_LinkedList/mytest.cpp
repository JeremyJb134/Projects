#include "csr.h"

class Tester{
    public:
        bool normalCompress(){
            cout << "Testing compress on a normal matrix" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, row*col);
            //checks to make sure the sizes of row, columns, and nonzeros are correct
            if(test.m_m != row or test.m_n != col or test.m_nonzeros != 4)
                return false;
            //checks to make sure the values of m_values are correct
            if(test.m_values[0] != 1 or test.m_values[1] != 2 or test.m_values[2] != 3 or test.m_values[3] != 4)
                return false;
            //checks to make sure the column indexes are correct
            if(test.m_col_index[0] != 0 or test.m_col_index[1] != 1 or test.m_col_index[2] != 2 or test.m_col_index[3] != 3)
                return false;
            //checks to make sure the row indexes are correct
            if(test.m_row_index[0] != 0 or test.m_row_index[1] != 1 or test.m_row_index[2] != 2 or test.m_row_index[3] != 3 
            or test.m_row_index[4] != 4)
                return false;
            return true;
        }
        bool smallerArrayError(){
            cout << "Testing compress with an array size smaller then matrix size" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 5;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            //checks to make sure the sizes of row, columns, and nonzeros are correct
            if(test.m_m != row or test.m_n != col or test.m_nonzeros != 4)
                return false;
            //checks to make sure the values of m_values are correct
            if(test.m_values[0] != 1 or test.m_values[1] != 2 or test.m_values[2] != 3 or test.m_values[3] != 4)
                return false;
            //checks to make sure the column indexes are correct
            if(test.m_col_index[0] != 0 or test.m_col_index[1] != 1 or test.m_col_index[2] != 2 or test.m_col_index[3] != 3)
                return false;
            //checks to make sure the row indexes are correct
            if(test.m_row_index[0] != 0 or test.m_row_index[1] != 1 or test.m_row_index[2] != 2 or test.m_row_index[3] != 3 
            or test.m_row_index[4] != 4)
                return false;
            //last row should only contain 0 since the array is smaller
            for(int i = 0; i < col; i++){
                if(test.getAt(4,i) != 0)
                    return false;
            }
            return true;
        }
        bool emptyMatrixError(){
            cout << "Testing compress with an empty" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 0;
            int col = 0;
            CSR test;
            test.compress(row, col, array, 16);
            //should create an empty object
            if(test.empty())
                return true;
            return false;
        }
        bool normalEqualTest(){
            cout << "Testing overloaded equality operator on normal case with both equal" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            //should create a CSR object with the same values
            CSR test2(test);
            //makes sure each array and variable are equal to each other
            if(test == test2){
                if(test.empty() and test2.empty())
                    return true;
                if(test.empty() or test2.empty())
                    return false;
                if(test.m_m != test2.m_m or test.m_n != test2.m_n or test.m_nonzeros != test2.m_nonzeros)
                    return false;
                for(int i = 0; i < test.m_nonzeros; i++){
                    if(test.m_values[i] != test2.m_values[i] or test.m_col_index[i] != test2.m_col_index[i])
                        return false;
                }
                for(int i = 0; i < (test.m_m + 1); i++){
                    if(test.m_row_index[i] != test2.m_row_index[i])
                        return false;
                }
            }
            return true;
        }
        bool normalUnequalTest(){
            cout << "Testing overloaded equality operator on normal case, where its not equal" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            int array2[] = {1,0,0,0,2,0,0,0,3};
            CSR test2;
            test2.compress(3,3,array2,9);
            //tests two different matrix to see if they have different elements
            if(!(test == test2)){
                if(test.empty() and test2.empty())
                    return false;
                if(test.empty() or test2.empty())
                    return true;
                if(test.m_m != test2.m_m or test.m_n != test2.m_n or test.m_nonzeros != test2.m_nonzeros)
                    return true;
                for(int i = 0; i < test.m_nonzeros; i++){
                    if(test.m_values[i] != test2.m_values[i] or test.m_col_index[i] != test2.m_col_index[i])
                        return true;
                }
                for(int i = 0; i < (test.m_m + 1); i++){
                    if(test.m_row_index[i] != test2.m_row_index[i])
                        return true;
                }
            }
            return false;

        }
        bool emptyEqualityTest(){
            cout << "Testing overloaded equality operator on empty object" << endl;
            CSR test;
            CSR test2;
            //returns true if they're both empty
            if(test == test2){
                if(test.empty() and test2.empty())
                    return true;
            }
            return false;
        }
        bool getAtError(){
            cout << "Testing getAt function at an index that doesn't exist" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            bool flag1 = false;
            bool flag2 = false;
            bool flag3 = false;
            test.compress(row, col, array, 16);
            //test for either row or column, or both is out of range
            try{
                test.getAt(5,3);
            } catch(const exception& e){
                flag1 = true;
            }
            try{
                test.getAt(0,5);
            } catch(const exception& e){
                flag2 = true;
            }
            try{
                test.getAt(6,6);
            } catch(const exception& e){
                flag3 =  true;
            }
            return(flag1 and flag2 and flag3);
        }
        bool listNormalEqual(){
            cout << "Testing assignment operator for normal case" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            int array2[] = {1,0,0,0,2,0,0,0,3};
            CSR test2;
            test2.compress(3,3,array2,9);
            CSRList testList; 
            CSRList testList2(testList); 
            //creates a CSRList with two nodes
            testList.insertAtHead(test);
            testList.insertAtHead(test2);
            //sets testList2 to equal testList
            //testList2 = testList;
            //CSR* cur = testList.m_head;
            //CSR* cur2 = testList2.m_head;
            //loops through both lists to make sure each node is equal
            return testList2 == testList;
        }
        bool listEmptyEqual(){
            cout << "Testing assignment operator for an empty object" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            int array2[] = {1,0,0,0,2,0,0,0,3};
            CSR test2;
            test2.compress(3,3,array2,9);
            CSRList testList; 
            CSRList testList2; 
            //creates a CSRList with two nodes
            testList.insertAtHead(test);
            testList.insertAtHead(test2);
            testList = testList2;
            //should return true if its empty
            return testList.empty();
        }
        bool listGetAtError(){
            cout << "Testing getAt for CSRList on an empty matrix" << endl;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            CSR test2;
            CSRList testList;
            testList.insertAtHead(test2);
            testList.insertAtHead(test);
            //trys to getAt at an empty matrix
            try{
                testList.getAt(1, 0, 0);
            }catch(const exception& e){
                return true;
            }
            return false;
        }
        bool listGetAtEmptyList(){
            cout << "Testing getAt for an empty list" << endl;
            CSRList testList;
            //trys to getAt at an empty list
            try{
                testList.getAt(0,0,0);
            }
            catch(const exception& e){
                return true;
            }
            return false;
        }
        bool listGetAtIndexError(){
            cout << "Testing getAt for an index out of range" << endl;
            CSRList testList;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            testList.insertAtHead(test);
            //trys to getAt at an index out of range
            try{
                testList.getAt(1, 0, 0);
            }
            catch(const exception& e){
                return true;
            }
            return false;
        }
        bool listNormalGetAt(){
            cout << "Testing getAt for a normal case" << endl;
            CSRList testList;
            int array[] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4};
            int row = 4;
            int col = 4;
            CSR test;
            test.compress(row, col, array, 16);
            testList.insertAtHead(test);
            bool flag = true;
            //checks the getAt for each nonzero in the matrix
            if(testList.getAt(0,0,0) != 1)
                flag = false;
            if(testList.getAt(0,1,1) != 2)
                flag = false;
            if(testList.getAt(0,2,2) != 3)
                flag = false;
            if(testList.getAt(0,3,3) != 4)
                flag = false;
            return flag;
        }
};

int main(){
    Tester test;
    if(test.normalCompress())
        cout << "Successful, compress on normal matrix created with correct values for each member variable" << endl;
    else 
        cout << "Unsuccessful, compress on normal matrix. Member variables not initialized correctly" << endl;
    cout << endl;
    if(test.smallerArrayError())
        cout << "Successful, compress on array size smaller then matrix size" << endl;
    else
        cout << "Unsuccessful, compress on smaller array then matrix" << endl;
    cout << endl;
    if(test.emptyMatrixError())
        cout << "Successful, empty matrix created even though array isn't empty" << endl;
    else
        cout << "Unsucessful, matrix not empty" << endl;
    cout << endl;
    if(test.normalEqualTest())
        cout << "Successful, equality operator returned correct value" << endl;
    else
        cout << "Unsuccessful, equality operator returned incorrect value" << endl;
    cout << endl;
    if(test.normalUnequalTest())
        cout << "Successful, equality operator returned false for unequal" << endl;
    else
        cout << "Unsuccessful, equality operatored returned incorrect value" << endl;
    cout << endl;
    if(test.getAtError())
        cout << "Successful, threw error for index out of range" << endl;
    else
        cout << "Unsuccessful, did not throw error for index out of range" << endl;
    cout << endl;
    if(test.listNormalEqual())
        cout << "Successful, assignment operator successfully copied the object" << endl;
    else
        cout << "Unsuccessful, assignment operator did not copy object" << endl;
    cout << endl;
    if(test.listEmptyEqual())
        cout << "Successful, assignment operator successfully created empty object" << endl;
    else
        cout << "Unsucessful, assignment oeprator did not create empty object" << endl;
    cout << endl;
    if(test.listGetAtError())
        cout << "Successful, getAt successfully threw an error" << endl;
    else
        cout << "Unsuccessful, getAt did not throw an error" << endl;
    cout << endl;
    if(test.listGetAtEmptyList())
        cout << "Successful, getAt threw an error for empty list" << endl;
    else
        cout << "Unsuccessful, getAt did not throw an error" << endl;
    cout << endl;
    if(test.listGetAtIndexError())
        cout << "Successful, getAt threw an error for index out of range" << endl;
    else
        cout << "Unsuccessful, getAt did not throw an error" << endl;
    cout << endl;
    if(test.listNormalGetAt())
        cout << "Successful, getAt got the correct values" << endl;
    else 
        cout << "Unsuccessful, getAt did not get correct values" << endl;
    return 0;
}