// CMSC 341 - Fall 2023 - Project 4
#include "dealer.h"
#include <random>
#include <vector>
#include <algorithm>
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

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

string carModels[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};

class Tester{
    public:
    bool testInsertion(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with different keys
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[i], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
        }
        bool flag = true;
        //inserts another 5 cars into the table with different keys
        for(int i = 5; i < 10; i++){
            int size = cardb.m_currentSize;
            Car dataObj = Car(carModels[i], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            int index = hashCode(carModels[i]) % cardb.m_currentCap;
            //calculates the index using the hash function and gets the car at that index
            //makes sure that car is equal to the car inserted, and also that the size is increased by one
            if(!(cardb.m_currentTable[index] == dataObj) or cardb.m_currentSize != size + 1)
                flag = false;
        }
        return flag;

    }
    bool testGetError(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with different keys
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[i], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
        }
        //makes sure getcar returns an empty object
        return (cardb.getCar("six", MINID).m_model == "" and cardb.getCar("six", MINID).m_used == false);
    }
    bool testGetNormal(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with the different keys
        for(int i = 0; i < 5; i++){
            int num = RndID.getRandNum();
            Car dataObj = Car(carModels[i], RndQuantity.getRandNum(), 
                          num, true);
            cardb.insert(dataObj);
            //trys to get the car using the keys
            if(!(cardb.getCar(carModels[i], num) == dataObj))
                return false;
        }
        return true;
    }
    bool testGetColliding(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with the same keys
        for(int i = 0; i < 5; i++){
            int num = RndID.getRandNum();
            Car dataObj = Car(carModels[0], RndQuantity.getRandNum(), 
                          num, true);
            cardb.insert(dataObj);
            //trys to get the car with the same key but different dealer
            if(!(cardb.getCar(carModels[0], num) == dataObj))
                return false;
        }
        return true;
    }
    bool removeNormal(){
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with different keys
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[i], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            //inserts the car into vector to keep track
            dataList.push_back(dataObj);
        }
        //trys to remove car from table
        for(int i = 0; i < 5; i++){
            if(cardb.remove(dataList[i]) != true)
                return false;
        }
        return true;
    }
    bool removeColliding(){
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        //inserts 5 cars into the table with different keys
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[0], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            dataList.push_back(dataObj);
        }
        for(int i = 0; i < 5; i++){
            if(cardb.remove(dataList[i]) != true)
                return false;
        }
        return true;
    }
    bool rehashLoadCheck(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,9);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        int size = 0;
        //inserts 51 cars which would cause load factor to be greater thatn 50%
        for(int i = 0; i < 51; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            size++;
        }
        size = size *4;
        //makes sure oldTable is inititalized and the correct cap
        return(cardb.m_oldTable != nullptr and cardb.m_currentCap == cardb.findNextPrime(size));
    }
    bool rehashLoad(){
        Random RndID(MINID,MAXID);
        Random RndCar(0,9);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        int size = 0;
        //inserts 51 cars which would cause load factor to be greater thatn 50%
        for(int i = 0; i < 51; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            size++;
        }
        //inserts another 5 to make sure all keys are transfered
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            size++;
        }
        //tests to see if oldTable is deallocated and the size is correct of current table
        return(cardb.m_oldTable == nullptr and cardb.m_currentSize == size);
    }
    bool rehashRemoveCheck(){
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,9);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        int size = 0;
        //inserts 51 cars which would cause load factor to be greater thatn 50%
        for(int i = 0; i < 50; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            dataList.push_back(dataObj);
            size++;
        }
        int deleted = 41;
        //removes 80% of cars + 1 extra to cause rehash
        for(int i = 0; i < deleted; i++){
            cardb.remove(dataList[i]);
            size--;
        }
        //checks to make sure oldTable is initialized and correct cap
        return(cardb.m_oldTable != nullptr and cardb.m_currentCap == cardb.findNextPrime(size));
    }
    bool rehashRemove(){
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,9);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        int size = 0;
        //inserts 51 cars which would cause load factor to be greater thatn 50%
        for(int i = 0; i < 50; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            dataList.push_back(dataObj);
            size++;
        }
        int deleted = 41;
        //removes 80% of cars + 1 extra to cause rehash
        for(int i = 0; i < deleted; i++){
            cardb.remove(dataList[i]);
            size--;
        }
        //inserts 5 nodes to completely transfer all data
        for(int i = 0; i < 5; i++){
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
            cardb.insert(dataObj);
            size++;
        }
        //makes sure oldTable is deallocated and correct size
        return(cardb.m_oldTable == nullptr and cardb.m_currentSize == size);
    }
};

int main(){
    Tester test;
    if(test.testInsertion()){
        cout << "Successful: Testing insertion with non colliding keys" << endl;
    }
    else{
        cout << "Unsuccessful: Testing insertion with non colliding keys" << endl;
    }
    if(test.testGetError()){
        cout << "Successful: Testing getCar for an error case" << endl;
    }
    else{
        cout << "Unsuccessful: Testing getCar for an error case" << endl;
    }
    if(test.testGetNormal()){
        cout << "Successful: Testing getCar with non colliding keys" << endl;
    }
    else{
        cout << "Unsuccessful: Testing getCar with non colliding keys" << endl;
    }
    if(test.testGetColliding()){
        cout << "Successful: Testing getCar with colliding keys" << endl;
    }
    else{
        cout << "Unsuccessful: Testing getCar with non colliding keys" << endl;
    }
    if(test.removeNormal()){
        cout << "Successful: Testing remove with non colliding keys" << endl;
    }
    else{
        cout << "Unsuccessful: Testing remove with non colliding keys" << endl;
    }
    if(test.removeColliding()){
        cout << "Successful: Testing remove with colliding keys" << endl;
    }
    else{
        cout << "Unsucessful: Testing remove with colliding keys" << endl;
    }
    if(test.rehashLoadCheck()){
        cout << "Successful: Testing rehashing after inserting a good amount of cars" << endl;
    }
    else{
        cout << "Unsuccessful: Testing rehashing after inserting a good amount of cars" << endl;
    }
    if(test.rehashLoad()){
        cout << "Successful: Testing rehash is completed after loadfactor reaches above 50%" << endl;
    }
    else{
        cout << "Unsuccessful: Testing rehash is completed after loadfactor reaches above 50%" << endl;
    }
    if(test.rehashRemoveCheck()){
        cout << "Successful: Testing rehash after removing a good amount of cars" << endl;
    }
    else{
        cout << "Unsuccessful: Testing rehashing after removing a good amount of cars" << endl;
    }
    if(test.rehashRemove()){
        cout << "Successful: Testing rehash is completed after deleteFactor reaches above 80%" << endl;
    }
    else{
        cout << "Unsuccessful: Testing rehash is completed after deleteFactor reaches above 80%" << endl;
    }
    return 0;
}