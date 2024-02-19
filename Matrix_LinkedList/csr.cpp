// CMSC 341 - Fall 2023 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    //initializes all member variables and creates empty object
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}
CSR::~CSR(){
    //deletes each array and sets variables to 0
    if(m_values != nullptr){
        delete[] m_values;
    }
    if(m_col_index != nullptr){
        delete[] m_col_index;
    }
    if(m_row_index != nullptr){
        delete[] m_row_index;
    }
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
   
}
CSR::CSR(const CSR & rhs){
    //checks to see if it isn't empty array
    if(rhs.m_values != nullptr){
        m_values = new int[rhs.m_nonzeros];
        //iterates through array and copies value at each index
        for(int i = 0; i < rhs.m_nonzeros; i++){
            m_values[i] = rhs.m_values[i];
        }
    }
    else
        m_values = nullptr;
    //checks to see if it isn't empty array
    if(rhs.m_col_index != nullptr){
        m_col_index = new int[rhs.m_nonzeros];
        //iterates through array and copies value at each index
        for(int i = 0; i < rhs.m_nonzeros; i++){
            m_col_index[i] = rhs.m_col_index[i];
        }
    }
    else   
        m_col_index = nullptr;
    if(rhs.m_row_index != nullptr){
        m_row_index = new int[rhs.m_m + 1];
        for(int i = 0; i < rhs.m_m + 1; i++){
            m_row_index[i] = rhs.m_row_index[i];
        }
    }
    else
        m_row_index = nullptr;
    //copies rows, size, and nonzeros 
    m_nonzeros = rhs.m_nonzeros;
    m_m = rhs.m_m;
    m_n = rhs.m_n;
    m_next = nullptr;

}
void CSR::clear(){
    //deletes each array and sets them to nullptr so it can be used later
    if(m_values != nullptr){
        delete[] m_values;
        m_values = nullptr;
    }
    if(m_col_index != nullptr){
        delete[] m_col_index;
        m_col_index = nullptr;
    }
    if(m_row_index != nullptr){
        delete[] m_row_index;
        m_row_index = nullptr;
    }
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}
bool CSR::empty() const{
    //returns true if each member variable is either null or 0
    if(m_values == nullptr and m_col_index == nullptr and m_row_index == nullptr 
    and m_nonzeros == 0 and m_m == 0 and m_n == 0 and m_next == nullptr){
        return true;
    }
    return false;
}
void CSR::compress(int m, int n, int array[], int arraySize){
    //clears object and sets the rows and columns to m and n
    clear();
    m_m = m;
    m_n = n;
    //only creates a matrix if m, n, arraySize aren't 0
    if(m != 0 and n != 0 and arraySize != 0){
        //sets row_index size to m+1 since the first index is always 0
        m_row_index = new int[m + 1];
        m_row_index[0] = 0;
        int size = m*n;
        //sets size to either size of row*columns or arraySize depending on which is smaller
        if(arraySize < m*n)
            size = arraySize;
        for(int i = 0; i < m*n; i++){
            //if not zero, increase m_nonzeros
            //makes sure i < size to make sure we're in size of array
            if(i < size){
                if(array[i] != 0)
                    m_nonzeros++;
            }
            if((i+1) % n == 0){
                //if the next index % n is 0, then that means a row is completed 
                m_row_index[(i+1) / n] = m_nonzeros;  
            }
        }
        //allocates col_index and values to size of nonzeros
        m_col_index = new int[m_nonzeros];
        m_values = new int[m_nonzeros];
        int counter = 0;
        for(int i = 0; i < size; i++){
            if(array[i] != 0){
                m_values[counter] = array[i];
                //does i % n to find which column the number is located
                m_col_index[counter] = i % n;
                counter++;
            }
        }
    }
    
}
int CSR::getAt(int row, int  col) const{
    //if row or column out of range, then throw error
    if(row > m_m or col > m_n or row < 0 or col < 0){
        throw runtime_error("Indices do not exist in matrix");
    }
    else if(empty())
        throw runtime_error("Error: Empty Object");
    else{
        //sets row_start to row_index at row
        int row_start = m_row_index[row];
        //incremenents row_start until it reaches row_index at row+1
        while(row_start != m_row_index[row + 1]){
            //returns the value if m_col_index equals the col
            if(m_col_index[row_start] == col)
                return m_values[row_start];
            row_start++;
        }
        //returns 0 otherwise
        return 0;
    }
} 
bool CSR::operator==(const CSR & rhs) const{
    //if they're both empty then they're equal
    if(empty() and rhs.empty())
        return true;
    //if one of them is empty then they are not equal
    if(empty() or rhs.empty())
        return false;
    //if the number of rows, columns, or nonzeros aren't equal then return false
    if(m_m != rhs.m_m or m_n != rhs.m_n or m_nonzeros != rhs.m_nonzeros)
        return false;
    //checks if the m_valus and m_col_index are equal at each index
    for(int i = 0; i < m_nonzeros; i++){
        if(m_values[i] != rhs.m_values[i] or m_col_index[i] != rhs.m_col_index[i])
            return false;
    }
    //checks if the row index is the same at each index
    for(int i = 0; i < (m_m + 1); i++){
        if(m_row_index[i] != rhs.m_row_index[i])
            return false;
    }
    return true;
}
int CSR::sparseRatio(){
    //checks to make sure rows and columns aren't 0
    if(m_m != 0 and m_n != 0){
        //finds size of array by doing rows * columns
        int numZeros = m_m * m_n;
        //finds the number of zeros by subtracting the number of nonzeros
        numZeros = numZeros - m_nonzeros;
        return (numZeros / (m_m * m_n));
    }
    return 0;
}

void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    //creates empty object
    m_head = nullptr;
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    //if its empty then create empty object
    if(rhs.empty()){
        m_head = nullptr;
        m_size = 0;
    }
    else{
        //iterates through both rhs and current
        m_head = nullptr;
        CSR* rhsCur = rhs.m_head;
        CSR* cur = m_head;
        while(rhsCur != nullptr){
            //creates a new CSR copying each CSR in rhs
            CSR* newCSR = new CSR(*rhsCur);
            //if m_head is nullptr then its the first node
            if(m_head == nullptr){
                m_head = newCSR;
                cur = m_head;
            }
            else{
                cur->m_next = newCSR;
                cur = cur->m_next; 
            }
            //sets cur and rhsCur to the next CSR
            rhsCur = rhsCur->m_next;
            m_size++;
        }
    }
}
CSRList::~CSRList(){
    //if its not empty, then iterate through list and delete each CSR
    if(!empty()){
        CSR* cur = m_head;
        while(cur != nullptr){
            m_head = cur;
            cur = cur->m_next;
            delete m_head;
        }
        m_head = nullptr;
        m_size = 0;
    }
}
bool CSRList::empty() const{
    //if head is null and size is 0, then its empty
    if(m_head == nullptr and m_size == 0)
        return true;
    return false;
}
void CSRList::insertAtHead(const CSR & matrix){
    if(empty()){
        //if its empty, then set m_head to the new CSR
        CSR* newNode = new CSR(matrix);
        m_head = newNode;
        m_size++;
    }
    else{
        //if its not empty set cur m_next to head, and then set head to cur
        CSR* cur = new CSR(matrix);
        cur->m_next = m_head;
        m_head = cur;
        m_size++;
    }
    
}
void CSRList::clear(){
    //if not empty, then iterate through list to delete each CSR
    if(!empty()){
        CSR* cur = m_head;
        while(cur != nullptr){
            m_head = cur;
            cur = cur->m_next;
            delete m_head;
        }
        m_head = nullptr;
        m_size = 0;
    }
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    if(empty())
        throw runtime_error("Error: Empty List");
    //if the index is greater then size then throw error
    else if((CSRIndex) > m_size - 1)
        throw runtime_error("Out of Range Error");
    else{
        //iterates through the list until it reaches CSRIndex
        int counter = 0;
        CSR* cur = m_head;
        while(counter != CSRIndex){
            counter++;
            cur = cur->m_next;
        }
        //gets the value at specified row and column
        return cur->getAt(row, col);
    }
}
bool CSRList::operator== (const CSRList & rhs) const{
    //if they're both empty, then they're the same
    if(rhs.empty() and empty())
        return true;
    //if only one is empty, then they're different
    if(rhs.empty() or empty())
        return false;
    //iterates through both lists to check if each CSR is the same 
    CSR* cur = m_head;
    CSR* rhsCur = rhs.m_head;
    while(rhsCur != nullptr){
        if(!(*rhsCur == *cur)){
            return false;
        }
        cur = cur->m_next;
        rhsCur = rhsCur->m_next;
    }
    return true;
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    //protect from self assignment
    if(this != &rhs){
        //if its empty, then create empty object
        if(rhs.empty()){
            clear();
        }
        else{
            //iterates through rhs to create a new node that is the same
            clear();
            CSR* rhsCur = rhs.m_head;
            CSR* cur = m_head;
            while(rhsCur != nullptr){
                CSR* newCSR = new CSR(*rhsCur);
                //sets to head if head is nullptr
                if(m_head == nullptr){
                    m_head = newCSR;
                    cur = m_head;
                }
                else{
                    cur->m_next = newCSR;
                    cur = cur->m_next;
                }
                rhsCur = rhsCur->m_next;
                m_size++;
            }
        }
    }
    return *this;
}
int CSRList::averageSparseRatio(){
    //if is not empty, calculate sparseRatio
    if(!empty()){
        int ratio = 0;
        CSR* cur = m_head;
        //iterates through whole list and adds up the sparseRatio of each CSR
        while(cur != nullptr){
            ratio += cur->sparseRatio();
            cur = cur->m_next;
        }
        //returns ratio divided by the size to get average
        return (ratio/m_size);
    }
    return 0;
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}