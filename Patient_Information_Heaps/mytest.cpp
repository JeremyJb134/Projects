// CMSC 341 - Fall 2023 - Project 3

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green", 
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong", 
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}

class Tester{
    public:
        bool insertNormalMin(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
            //inserts 300 random nodes
            for (int i=0;i<300;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            return checkHeap(aQueue, aQueue.m_heap);
        }
        bool insertNormalMax(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 300 random nodes
            for (int i=0;i<300;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            return checkHeap(aQueue, aQueue.m_heap);
        }
        bool checkHeap(PQueue& aQueue, Node* aHeap){
            bool result = true;
            //if nullptr then true
            if(aHeap == nullptr)
                return true;
            
            if(aQueue.m_heapType == MINHEAP){
                //if left exists, then priority should be less
                if(aHeap->m_left != nullptr and aHeap->m_right == nullptr)
                    result = (aQueue.m_priorFunc(aHeap->m_patient) <= aQueue.m_priorFunc(aHeap->m_left->m_patient));
                //if right exists, then priority should be less
                else if(aHeap->m_right != nullptr and aHeap->m_left == nullptr)
                    result = (aQueue.m_priorFunc(aHeap->m_patient) <= aQueue.m_priorFunc(aHeap->m_right->m_patient));
                else if(aHeap->m_right != nullptr and aHeap->m_left != nullptr){
                    result = (aQueue.m_priorFunc(aHeap->m_patient) <= aQueue.m_priorFunc(aHeap->m_right->m_patient))
                    and (aQueue.m_priorFunc(aHeap->m_patient) <= aQueue.m_priorFunc(aHeap->m_left->m_patient));
                }
                    
            }
            if(aQueue.m_heapType == MAXHEAP){
                //if left exists, then priority should be less
                if(aHeap->m_left != nullptr and aHeap->m_right == nullptr)
                    result = (aQueue.m_priorFunc(aHeap->m_patient) >= aQueue.m_priorFunc(aHeap->m_left->m_patient));
                //if right exists, then priority should be greater
                else if(aHeap->m_right != nullptr and aHeap->m_left == nullptr)
                    result = (aQueue.m_priorFunc(aHeap->m_patient) >= aQueue.m_priorFunc(aHeap->m_right->m_patient));
                else if(aHeap->m_right != nullptr and aHeap->m_left != nullptr){
                    result = (aQueue.m_priorFunc(aHeap->m_patient) >= aQueue.m_priorFunc(aHeap->m_right->m_patient))
                    and (aQueue.m_priorFunc(aHeap->m_patient) >= aQueue.m_priorFunc(aHeap->m_left->m_patient));
                }
                    
            }
            result = checkHeap(aQueue, aHeap->m_left);
            result = checkHeap(aQueue, aHeap->m_right);
            return result;
        }
        bool removeNormalMin(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
            for (int i=0;i<300;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            bool result = true;
            //tests to make sure the root is the node that is removed next
            while(aQueue.m_heap != nullptr){
                Node* temp = aQueue.m_heap;
                int tempNum = aQueue.m_priorFunc(temp->m_patient);
                result = (tempNum == aQueue.m_priorFunc(aQueue.getNextPatient()));
                //test to make sure the old root priority is less then the new root
                if(aQueue.m_heap != nullptr)
                    result = result and (tempNum <= aQueue.m_priorFunc(aQueue.m_heap->m_patient));
            }
            return result;
        }
        bool removeNormalMax(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            for (int i=0;i<300;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            bool result = true;
            //tests to make sure the root is the node that is removed next
            while(aQueue.m_heap != nullptr){
                Node* temp = aQueue.m_heap;
                int tempNum = aQueue.m_priorFunc(temp->m_patient);
                result = (tempNum == aQueue.m_priorFunc(aQueue.getNextPatient()));
                //test to make sure the old root priority is greater then the new root
                if(aQueue.m_heap != nullptr)
                    result = result and (tempNum >= aQueue.m_priorFunc(aQueue.m_heap->m_patient));
            }
            return result;
        }
        bool checkLeftist(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            return checkNPLRecurse(aQueue.m_heap);
        }
        bool checkLeftistRecurse(Node* aHeap){
            bool result = true;
            //if nullptr then true
            if(aHeap == nullptr)
                return true;
            //if left exists, and right doesn't, return true
            else if(aHeap->m_left != nullptr and aHeap->m_right == nullptr)
                result = true;
            //if right exists, and left doesn't, return false
            else if(aHeap->m_right != nullptr and aHeap->m_left == nullptr)
                return false;
            //makes sure left npl greater than right
            else if(aHeap->m_left != nullptr and aHeap->m_right != nullptr){
                result = result and (aHeap->m_left->m_npl >= aHeap->m_right->m_npl);
            }
            result = result and checkLeftistRecurse(aHeap->m_left);
            result = result and checkLeftistRecurse(aHeap->m_right);
            return result;
        }
        bool checkNPL(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 10 nodes to checkNPL
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            return checkNPLRecurse(aQueue.m_heap);
        }
        bool checkNPLRecurse(Node* aHeap){
            bool result = true;
            //checks to make sure each npl is correct in every node of tree
            if(aHeap != nullptr){
                result = (aHeap->m_npl == getNPL(aHeap));
                result = result and checkNPLRecurse(aHeap->m_left);
                result = result and checkNPLRecurse(aHeap->m_right);
            }
            return result;
        }
        int getNPL(Node* aHeap){
            if(aHeap != nullptr){
                //calculates the npl of current node and returns it
                int left = -1;
                int right = -1;
                if(aHeap->m_left != nullptr)
                  left = aHeap->m_left->m_npl;
                if(aHeap->m_right != nullptr){
                  right = aHeap->m_right->m_npl;
                }
                if(left < right){
                  Node* temp = aHeap->m_left;
                  aHeap->m_left = aHeap->m_right;
                  aHeap->m_right = temp;
                }
                int minHeight = (right < left) ? right : left;
                return minHeight + 1;
            }   
            return -1;              
        }
        bool changePriority(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue queue1(priorityFn2, MAXHEAP, LEFTIST);
            PQueue queue2(priorityFn1, MAXHEAP, LEFTIST);
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                queue1.insertPatient(patient);
                queue2.insertPatient(patient);
            }
            queue1.setPriorityFn(priorityFn1, MAXHEAP);
            bool result = true;
            //each node in the second queue should be equal to each node in the first queue after changing priortiy
            while(queue1.m_heap != nullptr){
                result = result and (queue1.getNextPatient() == queue2.getNextPatient());
            }
            return result;
        }
        bool checkMergeEdge(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 10 nodes to checkNPL
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            //checks to make sure that the size stays the same, which means no nodes were inserted
            PQueue aQueue2(priorityFn2, MAXHEAP, LEFTIST);
            int size = aQueue.m_size;
            aQueue.mergeWithQueue(aQueue2);
            return size == aQueue.m_size;
        }
        bool checkEqual(Node* aHeap, Node* otherHeap){
            if(aHeap != nullptr and otherHeap != nullptr){
                //checks to make each node has the same values
                bool flag = true;
                flag = flag and (aHeap->m_patient == otherHeap->m_patient);
                flag = flag and (aHeap->m_npl == otherHeap->m_npl);
                flag = flag and checkEqual(aHeap->m_left, otherHeap->m_left);
                flag = flag and checkEqual(aHeap->m_right, otherHeap->m_right);
                return flag;
            }
            //if both null then true
            else if(aHeap == nullptr and otherHeap == nullptr){
                return true;
            }
            //in any other then false
            return false;
        }
        bool checkCopyNormal(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 10 nodes to checkNPL
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            PQueue otherQueue(aQueue);
            aQueue.dump();
            otherQueue.dump();
            return checkEqual(aQueue.m_heap, otherQueue.m_heap);
        }
        bool checkCopyEdge(){
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            PQueue aQueue2(aQueue);
            //checks that each member variable is the same when copying an empty object
            bool result = true;
            result = (aQueue2.m_heap == nullptr and aQueue2.m_size == 0 and aQueue2.m_structure == aQueue.m_structure and 
            aQueue2.m_priorFunc == aQueue.m_priorFunc and aQueue2.m_heapType == aQueue.m_heapType);

            return result;
        }
        bool checkAssignmentNormal(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 10 nodes to checkNPL
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            PQueue otherQueue(priorityFn2, MAXHEAP, LEFTIST);
            otherQueue = aQueue;
            aQueue.dump();
            otherQueue.dump();
            //checks to make sure the heaps are equal
            return checkEqual(aQueue.m_heap, otherQueue.m_heap);
        }
        bool checkAssignmentEdge(){
            Random nameGen(0,NUMNAMES-1);
            Random temperatureGen(MINTEMP,MAXTEMP);
            Random oxygenGen(MINOX,MAXOX);
            Random respiratoryGen(MINRR,MAXRR);
            Random bloodPressureGen(MINBP,MAXBP);
            Random nurseOpinionGen(MINOPINION,MAXOPINION);
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //inserts 10 nodes to checkNPL
            for (int i=0;i<10;i++){
                Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
                aQueue.insertPatient(patient);
            }
            //assigns empty queue to full queue
            PQueue otherQueue(priorityFn2, MAXHEAP, LEFTIST);
            aQueue = otherQueue;
            return aQueue.m_heap == nullptr;
        }
        bool removeEmpty(){
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            //attempts to get next patinet for an empty queue
            try{
                aQueue.getNextPatient();
            }catch(const exception& e){
                return true;
            }
            return false;
        }
        bool mergeError(){
            PQueue aQueue(priorityFn2, MAXHEAP, LEFTIST);
            PQueue otherQueue(priorityFn1, MAXHEAP, LEFTIST);
            //attempts to merge queues with different priority functions
            try{
                aQueue.mergeWithQueue(otherQueue);
            }catch(const exception& e){
                return true;
            }
            return false;
        }

};

int main(){
    Tester test;
    if(test.insertNormalMin()){
        cout << "Successful: Inserting in a Normal Case for Min Heap" << endl;
    }
    else{
        cout << "Unsuccessful: Inserting in a Normal Case for Min Heap" << endl;
    }
    if(test.insertNormalMax()){
        cout << "Successful: Inserting in a Normal Case for Max Heap" << endl;
    }
    else{
        cout << "Unsuccessful: Inserting in a Normal Case for Max Heap" << endl;
    }
    if(test.removeNormalMin()){
        cout <<  "Successful: Removing in a Normal Case for Min Heap" << endl;
    }
    else{
        cout <<  "Unsuccessful: Removing in a Normal Case for Min Heap" << endl;
    }
    if(test.removeNormalMax()){
        cout <<  "Successful: Removing in a Normal Case for Max Heap" << endl;
    }
    else{
        cout <<  "Unsuccessful: Removing in a Normal Case for Max Heap" << endl;
    }
    if(test.checkNPL()){
        cout << "Successful: Testing NPL values of a Heap" << endl;
    }
    else{
        cout << "Unsuccessful: Testing NPL value of a Heap" << endl;
    }
    if(test.checkLeftist()){
        cout << "Successful: Testing NPL values of Leftist Heap" << endl;
    }
    else{
        cout << "Unsuccessful: Testing NPL values of Leftist Heap" << endl;
    }
    if(test.changePriority()){
        cout << "Successful: Testing changing the priority of Heap" << endl;
    }
    else{
        cout << "Unsuccessful: Testing changing the priortiy of Heap" << endl;
    }
    if(test.checkMergeEdge()){
        cout << "Successful: Testing merge with an empty object" << endl;
    }
    else{
        cout << "Unsuccessful: Testing merge with and empty object" << endl;
    }
    if(test.checkCopyNormal()){
        cout << "Successful: Testing copy constructor in normal case" << endl;
    }
    else{
        cout << "Unsuccessful: Testing copy constructor in normal case" << endl;
    }
    if(test.checkCopyEdge()){
        cout << "Successuful: Testing copy constructor in edge case" << endl;
    }
    else{
        cout << "Unsuccessuful: Testing copy constructor in edge case" << endl;
    }
    if(test.checkAssignmentNormal()){
        cout << "Successuful: Testing assignment operator in normal case" << endl;
    }
    else{
        cout << "Unsuccessuful: Testing assignment operator in normal case" << endl;
    }
    if(test.checkAssignmentEdge()){
        cout << "Successuful: Testing assignment operator in edge case" << endl;
    }
    else{
        cout << "Unsuccessuful: Testing assignment operator in edge case" << endl;
    }
    if(test.removeEmpty()){
        cout << "Successful: Testing throwing an error for removing from empty queue" << endl;
    }
    else{
        cout << "Unsuccessful: Testing throwing an error for removing from empty queue" << endl;
    }
    if(test.mergeError()){
        cout << "Successful: Testing throwing an error for merging two queues with different priority" << endl;
    }
    else{
        cout << "Unsuccessful: Testing throwing an error for merging two queues with different priority" << endl;
    }
    return 0;
}