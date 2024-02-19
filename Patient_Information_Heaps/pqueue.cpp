// CMSC 341 - Fall 2023 - Project 3
//Name: Jeremy Bruce
//Email: jbruce2@umbc.edu
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
  //sets each member variable
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_size = 0;
  m_heap = nullptr;
}
PQueue::~PQueue()
{
  //calls clear
  clear();
}
void PQueue::clear() {
  //deletes each node and sets heap to null and size to 0
  postOrder(m_heap);
  m_heap = nullptr;
  m_size = 0;

}
PQueue::PQueue(const PQueue& rhs)
{
  //recursiviely copies heap
  m_heap = nullptr;
  m_structure = rhs.m_structure;
  m_heapType = rhs.m_heapType;
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  if(rhs.m_heap != nullptr){
    preOrder(m_heap, rhs.m_heap);
  }
}

PQueue& PQueue::operator=(const PQueue& rhs) {
  if (this != &rhs){
    m_structure = rhs.m_structure;
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.m_priorFunc;
    //if null then just clear heap
    if(rhs.m_heap == nullptr)
      clear();
    //if not null then recursively copy heap
    else{
      clear();
      preOrder(m_heap, rhs.m_heap);
      m_size = rhs.m_size;
    }
  }
  return *this;
  
}
void PQueue::mergeWithQueue(PQueue& rhs) {
  if(m_priorFunc == rhs.m_priorFunc and m_structure == rhs.m_structure and this != &rhs){
    //merges both of the queues roots
    m_heap = mergeRecur(m_heap, rhs.m_heap);
    //increase size of left heap by right heap
    m_size += rhs.m_size;
    rhs.clear();
  }
  else
    throw domain_error("Not the same structure or priority");
}

void PQueue::insertPatient(const Patient& patient) {
  if(m_heap == nullptr){
    //creates a new patient that is now the root
    Node* newPatient = new Node(patient);
    m_heap = newPatient;
    m_size++;
  }
  else{
    //merges the current heap with new patient
    Node* newPatient = new Node(patient);
    mergeRecur(m_heap, newPatient);
    m_size++;
  }
}

int PQueue::numPatients() const
{
  //returns size
  return m_size;
}

prifn_t PQueue::getPriorityFn() const {
  //returnrs priority
  return m_priorFunc;
}
Patient PQueue::getNextPatient() {
    if(m_heap != nullptr){
      //stores m_heap and then merges left and right children
      Node* temp = m_heap;
      m_heap = mergeRecur(m_heap->m_left, m_heap->m_right);
      m_size--;
      Patient tempPatient(temp->m_patient);
      //deletes temp
      delete temp;
      return tempPatient;
    }
    else{
      throw out_of_range("Out of Range Error");
    }
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  //sets priortiy and heaptype
  m_priorFunc = priFn;
  m_heapType = heapType;
  //creates a new queue with correct priortiy and inserts each node into new queue
  PQueue tempQueue(m_priorFunc, m_heapType, m_structure);
  while(m_heap != nullptr and m_size != 0){
    tempQueue.insertPatient(getNextPatient());
  }
  //sets current queue to equal the temp queue
  *this = tempQueue;
}

void PQueue::setStructure(STRUCTURE structure){
  m_structure = structure;
  PQueue tempQueue(m_priorFunc, m_heapType, m_structure);
   //creates a new queue with correct structure and inserts each node into new queue
  while(m_heap != nullptr){
    tempQueue.insertPatient(getNextPatient());
  }
  //sets current queue to equal the temp queue
  *this = tempQueue;
}

STRUCTURE PQueue::getStructure() const {
  //returns structure
  return m_structure;
}

HEAPTYPE PQueue::getHeapType() const {
  //returns heaptype
  return m_heapType;
}

void PQueue::printPatientQueue() const {
  //prints each node recursively
  cout << "Contents of the queue:" << endl;
  preOrderPrint(m_heap);
}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
void PQueue::postOrder(Node* aHeap){
  //goes through each node and deletes
  if(aHeap != nullptr){
        postOrder(aHeap->m_left);
        postOrder(aHeap->m_right);
        delete aHeap;
        aHeap = nullptr;
    }
}
void PQueue::preOrder(Node*& aHeap, const Node* rhs){
   if(rhs != nullptr){
        Node* newNode = new Node(rhs->m_patient);
        newNode->setNPL(rhs->m_npl);
        //sets aHeap to point to new heap object
        aHeap = newNode;
        //moves left and right
        preOrder(aHeap->m_left, rhs->m_left);
        preOrder(aHeap->m_right, rhs->m_right);
    }
}
Node* PQueue::mergeRecur(Node*& leftHeap, Node*& rightHeap){
  //if left is null return right
  if(leftHeap == nullptr)
    return rightHeap;
  //if right is null return left
  else if (rightHeap == nullptr)
    return leftHeap;
  else{
    //if type is min and left priority is greater then right, then swap
    if(m_heapType == MINHEAP){
      if(m_priorFunc(leftHeap->m_patient) > m_priorFunc(rightHeap->m_patient)){
        Node* temp = leftHeap;
        leftHeap = rightHeap;
        rightHeap = temp;
      }
    }
    //if type is max and left priority is less then right, then swap
    else{
      if(m_priorFunc(leftHeap->m_patient) < m_priorFunc(rightHeap->m_patient)){
        Node* temp = leftHeap;
        leftHeap = rightHeap;
        rightHeap = temp;
      }
    }
    //keeps track of node to return
    Node* returnHeap = leftHeap;
    //if structure is skew then simply swap children and then merge right heap with left child
    if(m_structure == SKEW){
      Node* temp = leftHeap->m_right;
      leftHeap->m_right = leftHeap->m_left;
      leftHeap->m_left = temp;
      leftHeap->m_left = mergeRecur(leftHeap->m_left, rightHeap);
      returnHeap = leftHeap;
    }
    else{
      //if right is null then set right to rightHeap
      if(leftHeap->m_right == nullptr){
        leftHeap->m_right = rightHeap;
      }
      else{
        //if type is minHeap and left priority is less then right, then merge left right child with right heap
        if(m_heapType == MINHEAP){
          if(m_priorFunc(leftHeap->m_patient) <= m_priorFunc(rightHeap->m_patient)){
            leftHeap->m_right = mergeRecur(leftHeap->m_right, rightHeap);
          }
          //if greater then right, then right becomes new root and merge right of right heap with left heap
          else{
            rightHeap->m_right = mergeRecur(rightHeap->m_right, leftHeap);
            returnHeap = rightHeap;
          }
        }
         //if type is maxHeap and left priority is greater then right, then merge left right child with right heap
        else{
          if(m_priorFunc(leftHeap->m_patient) >= m_priorFunc(rightHeap->m_patient)){
            leftHeap->m_right = mergeRecur(leftHeap->m_right, rightHeap);
          }
          //if less then right, then right becomes new root and merge right of right heap with left heap
          else{
            rightHeap->m_right = mergeRecur(rightHeap->m_right, leftHeap);
            returnHeap = rightHeap;
          }
        }
      }
      //recalculates NPL starting from node we merged at
      recalculateNPL(returnHeap);
    }
    return returnHeap;
  }
}
void PQueue::recalculateNPL(Node*& aHeap){
  if(aHeap != nullptr){
    //calculaes npl starting from the leaf node
    recalculateNPL(aHeap->m_left);
    recalculateNPL(aHeap->m_right);
    int left = -1;
    int right = -1;
    //sets left to equal npl of left child
    if(aHeap->m_left != nullptr)
      left = aHeap->m_left->m_npl;
    //sets right to equal npl of right child
    if(aHeap->m_right != nullptr){
      right = aHeap->m_right->m_npl;
    }
    //if left is less than right, then swap children
    if(left < right){
      Node* temp = aHeap->m_left;
      aHeap->m_left = aHeap->m_right;
      aHeap->m_right = temp;
    }
    //sets the root npl to equal 1 + the smaller of left and right
    int minHeight = (right < left) ? right : left;
    aHeap->setNPL(minHeight + 1);
  }
}
void PQueue::preOrderPrint(Node* aHeap) const{
  //goes through each node in preOrder, and prints its content
  if(aHeap != nullptr){
    cout << '[' << m_priorFunc(aHeap->m_patient) << "] " << aHeap->m_patient.getPatient() << ", temperature: " << 
    aHeap->m_patient.getTemperature() << ", oxygen: " << aHeap->m_patient.getOxygen() << ", respiratory: " <<
    aHeap->m_patient.getBP() << ", nurse opinion: " << aHeap->m_patient.getOpinion() << endl;
    preOrderPrint(aHeap->m_left);
    preOrderPrint(aHeap->m_right);
  }
}