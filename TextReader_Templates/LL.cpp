/***********************************************************
 ** File:    LL.cpp
 ** Project: CMSC 202 - Project 5
 ** Author:  Jeremy Bruce
 ** Date     5/2/2023
 ** Section: 11
 **********************************************************/


#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

//DO NOT ADD ANY CONSTANTS OR FUNCTIONS TO ANY CLASS

// Templated Node class
// Normal node except that each node can hold a pair <T,int>
// where the first value is templated and the second value is the frequency
// (or quantity) of the first
// For example, if T was a string then it would hold string,int
template <class T>
class Node {
public:
  Node( const T& data ); //Node Constructor - Assume each quan
  pair<T,int>& GetData(); //Node Data Getter
  void SetData( const pair<T,int>& ); //Node Data Setter
  Node<T>* GetNext(); //Node m_next getter
  void SetNext( Node<T>* next ); //Node m_next setter
private:
  pair <T,int> m_data; //Holds templated data
  Node<T>* m_next; //Pointer to next node
};

template <class T>
Node<T>::Node( const T& data ) {
  m_data = make_pair(data,1);
  m_next = nullptr;
}

template <class T>
pair<T,int>& Node<T>::GetData() {
   return m_data;
}

template <class T>
void Node<T>::SetData( const pair<T,int>& data ) {
   m_data = data;
}

template <class T>
Node<T>* Node<T>::GetNext() {
   return m_next;
}

template <class T>
void Node<T>::SetNext( Node<T>* next ) {
   m_next = next;
}

//Templated Linked List class
template <class T>
class LL {
 public:
  // Name: LL() (Linked List) - Default Constructor
  // Desc: Used to build a new linked list
  // Preconditions: None
  // Postconditions: Creates a new linked list where m_head points to nullptr
  LL();
  // Name: ~LL() - Destructor
  // Desc: Used to destruct a LL
  // Preconditions: There is an existing LL with at least one node
  // Postconditions: A LL is deallocated (including dynamically allocated nodes)
  //                 to have no memory leaks!
 ~LL();
  // Name: LL (Copy Constructor)
  // Desc: Creates a copy of existing LL
  //       Requires a LL - REQUIRED to be implemented even if not used
  // Preconditions: Source LL exists
  // Postconditions: Copy of source LL
  LL(const LL&);
  // Name: operator= (Overloaded Assignment Operator)
  // Desc: Makes two LL of the same type have identical number of nodes and values
  // Preconditions: Requires two linked lists of the same templated type
  //                REQUIRED to be implemented even if not used
  // Postconditions: Two idenetical LL
  LL<T>& operator= (const LL&);
  // Name: Find
  // Desc: Iterates through LL and returns node if data found
  // Preconditions: LL Populated
  // Postconditions: Returns nullptr if not found OR Node pointer if found
  Node<T>* Find(const T& data);
  // Name: Insert
  // Desc: Either inserts a node to the linked list OR increments frequency of first
  //       Takes in data. If "first" (of pair) NOT in list, adds node
  //       If "first" (of pair) is already in list, increments quantity
  //       Inserts "first" in order with no duplicates
  // Preconditions: Requires a LL.
  // Postconditions: Node inserted in LL based on first value (or quantity incremented)
  void Insert(const T&);
  // Name: RemoveAt
  // Desc: Removes a node at a particular position based on data passed (matches first)
  // Preconditions: LL with at least one node. 
  // Postconditions: Removes first node with passed value (in first)
  void RemoveAt(const T&);
  // Name: Display
  // Desc: Display all nodes in linked list
  // Preconditions: Outputs the LL
  // Postconditions: Displays the pair in each node of LL 
  void Display();
  // Name: GetSize
  // Desc: Returns the size of the LL
  // Preconditions: Requires a LL
  // Postconditions: Returns m_size
  int GetSize();
  // Name: operator<< (Overloaded << operator)
  // Desc: Returns the ostream of the data in each node
  // Preconditions: Requires a LL
  // Postconditions: Returns an ostream with the data from each node on different line
  template <class U>
  friend ostream& operator<<(ostream& output, const LL<U>&);
  // Name: Overloaded [] operator
  // Desc: When passed an integer, returns the data at that location
  // Precondition: Existing LL
  // Postcondition: Returns pair from LL using []
  pair<T,int>& operator[] (int x);//Overloaded [] operator to pull data from LL
private:
  Node <T> *m_head; //Node pointer for the head
  int m_size; //Number of nodes in queue
};
//*****************************************************************
//Implement LL here

template <class T>
LL<T>::LL(){
  m_head = nullptr; //sets head to nullptr and size to 0
  m_size = 0;
}

template <class T>
LL<T>::~LL(){
  //iterates through the list starting from m_head and deletes each node
  Node<T>* cur = m_head;
  while(cur != nullptr){
    m_head = cur;
    cur = cur->GetNext();
    delete m_head;
  }  
  m_size = 0;
  m_head = nullptr;
}

  // Name: LL (Copy Constructor)
  // Desc: Creates a copy of existing LL
  //       Requires a LL - REQUIRED to be implemented even if not used
  // Preconditions: Source LL exists
  // Postconditions: Copy of source LL
template <class T>
LL<T>::LL(const LL& list){
  //ititializes m_head and size
  //creates two pointers to the beginnings of both lists
  Node<T>* curList = list.m_head;
  m_head = nullptr;
  m_size = 0;
  Node<T>* cur = m_head;
  while(curList != nullptr){
    //creates a new node using the data from each node in list
    Node<T>* newNode = new Node<T>(curList->GetData().first);
    newNode->SetData(make_pair(curList->GetData().first, curList->GetData().second));
    if (m_head == nullptr){ //if m_head is nullptr makes m_head point to newNode
      m_head = newNode;
      cur = newNode;
      curList = curList->GetNext();
    }
    else{ //if not sets the current node to point at newNode
      cur->SetNext(newNode);
      cur = newNode;
      curList = curList->GetNext();
    }
    m_size++; //increments size by 1
  }
}

  // Name: operator= (Overloaded Assignment Operator)
  // Desc: Makes two LL of the same type have identical number of nodes and values
  // Preconditions: Requires two linked lists of the same templated type
  //                REQUIRED to be implemented even if not used
  // Postconditions: Two idenetical LL
template <class T>
LL<T>& LL<T>::operator= (const LL& list){
  if(this != &list){
    //deletes all the nodes in the current list
    Node<T>* cur = m_head;
    while(cur != nullptr){
      m_head = cur;
      cur = cur->GetNext();
      delete m_head;
    }  
    //sets the size to 0 and head to nullptr
    m_size = 0;
    m_head = nullptr;
    Node<T>* curList = list.m_head;
    cur = m_head;
    //loops through whole list
    while(curList != nullptr){
      //creates a new node using the data for each node in list
      Node<T>* newNode = new Node<T>(curList->GetData().first);
      newNode->SetData(make_pair(curList->GetData().first, curList->GetData().second));
      if (m_head == nullptr){ //if m_head is nullptr makes m_head point to newNode
        m_head = newNode;
        cur = newNode;
        curList = curList->GetNext();
      }
      else{ //if not sets the current node to point at newNode
        cur->SetNext(newNode);
        cur = newNode;
        curList = curList->GetNext();
      }
      m_size++; //increments size by 1
    }
  }
  return *this; //returns the new list at the end
}

  // Name: Find
  // Desc: Iterates through LL and returns node if data found
  // Preconditions: LL Populated
  // Postconditions: Returns nullptr if not found OR Node pointer if found
template <class T>
Node<T>* LL<T>::Find(const T& data){
  Node<T>* cur = m_head;
  //iterates through whole linked list
  while(cur != nullptr){
    //if the first data is equal to data then return node
    if (cur->GetData().first == data)
      return cur;
    else
      cur = cur->GetNext();
  }
  return cur; //returns nullptr if not found
}

  // Name: Insert
  // Desc: Either inserts a node to the linked list OR increments frequency of first
  //       Takes in data. If "first" (of pair) NOT in list, adds node
  //       If "first" (of pair) is already in list, increments quantity
  //       Inserts "first" in order with no duplicates
  // Preconditions: Requires a LL.
  // Postconditions: Node inserted in LL based on first value (or quantity incremented)
template <class T>
void LL<T>::Insert(const T& data){
  Node<T>* cur = Find(data); //checks to see if the node is in the list already
  if(cur != nullptr){
    //if it is, icnreases the second data by 1
    pair<T,int> newData = cur->GetData();
    newData.second += 1;
    cur->SetData(newData);
  }
  else{
    Node<T>* newNode = new Node<T>(data);
    if (m_size == 0){
      //if size is 0, lets m_head to the new node
      m_head = newNode;
      m_size++;
    }
    else{
      cur = m_head; //the new node will point to this
      Node<T>* prev = m_head; //will point to the new node
      bool flag = true; //stops the loop when location to insert is found
      while((flag == true) and (cur != nullptr)){ //if cur is nullptr then add node to end of list
        if (data < cur->GetData().first) //if data is less then cur data then, insert befor cur
          flag = false;
        else{
          prev = cur; //if not, sets prev to cur and cur to the next node
          cur = cur->GetNext();
        }
      }
      newNode->SetNext(cur); //sets the new node to point to cur
      if (cur == m_head) //if cur is head, then insert at beginning by changing m_head
        m_head = newNode;
      else
        prev->SetNext(newNode); //if not, set prev to point to new node
      m_size++;
    }
  }
}

  // Name: RemoveAt
  // Desc: Removes a node at a particular position based on data passed (matches first)
  // Preconditions: LL with at least one node. 
  // Postconditions: Removes first node with passed value (in first)
template <class T>
void LL<T>::RemoveAt(const T& data){
  //creates two pointers to keep track of before and after
  Node<T>* cur = m_head;
  Node<T>* prev = m_head;
  if(cur->GetData().first == data){
    m_head = cur->GetNext();
    delete cur;
  }
  else{
    while(cur->GetData().first != data){ //stops when first data equals data
      prev = cur;
      cur = cur->GetNext();
    }
    prev->SetNext(cur->GetNext()); //sets the previous node to point to cur next node
    delete cur; //deletes cur and reduces size
  }
  m_size--;
}

  // Name: Display
  // Desc: Display all nodes in linked list
  // Preconditions: Outputs the LL
  // Postconditions: Displays the pair in each node of LL 
template <class T>
void LL<T>::Display(){
  Node<T>* cur = m_head; //iterates through whole list and couts each node data
  while(cur != nullptr){
    cout << cur->GetData().first << ":" << cur->GetData().second << endl;
    cur = cur->GetNext();
  }
  cout << "END" << endl;
}

  // Name: GetSize
  // Desc: Returns the size of the LL
  // Preconditions: Requires a LL
  // Postconditions: Returns m_size
template <class T>
int LL<T>::GetSize(){
  return m_size;
}

  // Name: operator<< (Overloaded << operator)
  // Desc: Returns the ostream of the data in each node
  // Preconditions: Requires a LL
  // Postconditions: Returns an ostream with the data from each node on different line
template <class U>
ostream& operator<<(ostream& output, const LL<U>& list){
  Node<U>* cur = list.m_head; //sets cur to the list head
  while(cur != nullptr){ //iterates through whole list and couts each node data
    output << cur->GetData().first << ":" << cur->GetData().second << endl;
    cur = cur->GetNext();
  }
  output << "END" << endl;
  return output;
}

  // Name: Overloaded [] operator
  // Desc: When passed an integer, returns the data at that location
  // Precondition: Existing LL
  // Postcondition: Returns pair from LL using []
template <class T>
pair<T,int>& LL<T>::operator[] (int x){
  int counter = 0;
  //creates a counter to keep track of node indexes
  Node<T>* cur = m_head;
  while(counter != x){ //stops when the counter equals x
    cur = cur->GetNext();
    counter++; //increments counter for each loop
  }
  return cur->GetData(); //returns the data
}









