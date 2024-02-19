// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

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

class Tester{
    public:
        bool insertNormal(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(MINID+i, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            bool flag = false;
            //checks to make sure each sat is in the tree
            for(int i = 0; i < size; i++){
                flag = satList.findSatellite(MINID+i);
                if (flag == false)
                    return flag;
            }
            return flag;
        }
        bool insertEdge(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(50000+i, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            Sat satMin(MINID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            satList.insert(satMin);
            Sat satMax(MAXID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            satList.insert(satMax);
            //checks to make sure max and min sat is in the tree
            return (satList.findSatellite(MINID) and satList.findSatellite(MAXID));
        }
        bool insertErrorRange(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(MINID+i, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            Sat satellite(MINID - 1, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            satList.insert(satellite);
            return (!satList.findSatellite(MINID - 1));
        }
        bool checkBST(Sat* satellite){
            bool result = true;
            //if nullptr then true
            if(satellite == nullptr)
                return true;
            //if left exists, then id should be greater
            else if(satellite->m_left != nullptr and satellite->m_right == nullptr)
                result = (satellite->m_id > satellite->m_left->m_id);
            //if right exists, then id should be less
            else if(satellite->m_right != nullptr and satellite->m_left == nullptr)
                result = (satellite->m_id < satellite->m_right->m_id);
            else if(satellite->m_left != nullptr and satellite->m_right != nullptr)
                result = (satellite->m_id > satellite->m_left->m_id) and (satellite->m_id < satellite->m_right->m_id);
            result = checkBST(satellite->m_left);
            result = checkBST(satellite->m_right);
            return result;
        }
        bool checkAVL(Sat* satellite){
            bool result = true;
            //if nullptr then true
            if(satellite == nullptr)
                return true;
            //if left exists, and right doesn't, then height of left should be 0
            else if(satellite->m_left != nullptr and satellite->m_right == nullptr)
                result = (satellite->m_left->m_height == 0);
            //if right exists, and left doesn't, then height of right should be 0
            else if(satellite->m_right != nullptr and satellite->m_left == nullptr)
                result = (satellite->m_right->m_height == 0);
            //makes sure the difference in height isn't greater than 1
            else if(satellite->m_left != nullptr and satellite->m_right != nullptr){
                int diff = satellite->m_left->m_height - satellite->m_right->m_height;
                result = (diff >= -1 and diff <= 1);
            }
            result = checkAVL(satellite->m_left);
            result = checkAVL(satellite->m_right);
            return result;
        }
        bool insertMany(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            Random idGen(MINID,MAXID);
            int size = 300;
            //trys to insert 300 nodes
            for(int i = 0; i < size; i++){
                Sat satellite(idGen.getRandNum(), static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);         
            }
            //checks the bst and AVL properties
            return (checkBST(satList.m_root) and checkAVL(satList.m_root));
        }
        bool removeNormal(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            Random idGen(MINID,MINID + 9);
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(MINID+i, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            //removes a random node that exists
            int ID = idGen.getRandNum();
            satList.remove(ID);
            return !(satList.findSatellite(ID));
        }
        bool removeEdge(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            //inserts 10 Sats into the tree
            Random idGen(MINID,MAXID);
            int ID = idGen.getRandNum();
            Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            //removes node when there is only 1 node
            satList.remove(ID);
            return (satList.m_root == nullptr);
        }
        bool removeMany(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            Random idGen(MINID,MAXID);
            vector<int> idVector;
            int size = 300;
            //trys to insert 300 nodes
            int ID = 0;
            for(int i = 0; i < size; i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
                idVector.push_back(ID);
            }
            //removes half of the nodes
            for(int i = 0; i < size/2; i++){
                satList.remove(idVector[i]);
            }
            //checks the bst and AVL properties
            return (checkBST(satList.m_root) and checkAVL(satList.m_root));
        }
        bool checkRemoveDeorbited(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(MINID+i, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
                //sets half of them to be DEORBITED
                if(i % 2 == 0)
                    satList.setState(MINID+i, DEORBITED);
            }
            satList.removeDeorbited();
            //count should be 0
            return (satList.countSatellitesDeorbited(satList.m_root) == 0);
        }
        bool checkCountSatellites(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            int size = 10;
            //inserts 10 Sats into the tree
            for(int i = 0; i < size; i++){
                Sat satellite(MINID+i, static_cast<ALT>(altGen.getRandNum()));
                //sets half to have incline corresponding to 1
                if(i % 2 == 0)
                    satellite.setInclin(I53);
                satList.insert(satellite);
            }
            return(satList.countSatellites(I53) == size/2);
        }
        bool checkFindNormal(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            Random idGen(MINID,MAXID);
            int size = 300;
            int IDSave = 0;
            int ID = 0;
            //trys to insert 300 nodes
            for(int i = 0; i < size; i++){
                ID = idGen.getRandNum();
                if(i == size/2)
                    IDSave = ID;
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            return(satList.findSatellite(IDSave));
        }
        bool checkFindError(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            Random idGen(MINID,MAXID);
            int size = 300;
            int ID = 0;
            //trys to insert 300 nodes
            for(int i = 0; i < size; i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            return(!satList.findSatellite(100));
        }
        bool assignmentNomal(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3); 
            SatNet satList;
            Random idGen(MINID,MAXID);
            int size = 300;
            int ID = 0;
            //trys to insert 300 nodes
            for(int i = 0; i < size; i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList.insert(satellite);
            }
            SatNet satList2;
            satList2 = satList;
            return(checkEqual(satList.m_root, satList2.m_root));
        }
        bool checkEqual(Sat* satellite, Sat* satellite2){
            //checks to make sure each part of sat is equal
            if(satellite != nullptr and satellite2 != nullptr){
                bool flag = true;
                flag = (satellite->getID() == satellite2->getID());
                flag = flag and (satellite->getInclin() == satellite2->getInclin());
                flag = flag and (satellite->getState() == satellite2->getState());
                flag = flag and (satellite->getAlt() == satellite2->getAlt());
                flag = flag and (satellite->getHeight() == satellite2->getHeight());
                flag = flag and checkEqual(satellite->m_left, satellite2->m_left);
                flag = flag and checkEqual(satellite->m_right, satellite2->m_right);
                return flag;
            }
            //if one is null and other is not, then not equal
            else if(satellite != nullptr and satellite2 == nullptr)
                return false;
            //if both null then equal
            else
                return true;
        }
        bool assignmentError(){
            SatNet satList;
            SatNet satList2;
            satList = satList2;
            return(satList.m_root == nullptr and satList2.m_root == nullptr);
        }
        bool timeMeasurementInsert(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3);
            Random idGen(MINID,MAXID);
            double time1;
            clock_t start1, stop1;
            int size = 1000;
            int ID = 0;
            SatNet satList1;
            //calculates time for inserting 1000 nodes
            start1 = clock();
            for(int i=0;i<size;i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList1.insert(satellite);
            }
            stop1 = clock();
            time1 = stop1 - start1;
            time1 = time1/CLOCKS_PER_SEC;
            double time2;
            clock_t start2, stop2;
            SatNet satList2;
            //calculates time for inserting 2000 nodes
            start2 = clock();
            for(int i=0;i<size*2;i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList2.insert(satellite);
            }
            //calculates the ratio of time2 and time1
            stop2 = clock();
            time2 = stop2 - start2;
            time2 = time2/CLOCKS_PER_SEC;
            int ratio = time2/(2*time1);
            //returns if the ratio is in the range
            return ((1.1 - 0.4) <= ratio and (1.1 + 0.4) >= ratio);
        }
        bool timeMeasurementRemove(){
            Random inclinGen(0,3);  // there are 4 inclination
            Random altGen(0,3);
            Random idGen(MINID,MAXID);
            double time1;
            clock_t start1, stop1;
            int size = 1000;
            int ID = 0;
            SatNet satList1;
            vector<int> idList1;
            for(int i=0;i<size;i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList1.insert(satellite);
                idList1.push_back(ID);
            }
            //calculates time for removing 1000 nodes
            start1 = clock();
            for(int i = 0; i < size; i++){
                satList1.remove(idList1[i]);
            }
            stop1 = clock();
            time1 = stop1 - start1;
            time1 = time1/CLOCKS_PER_SEC;
            double time2;
            clock_t start2, stop2;
            SatNet satList2;
            vector<int> idList2;
            for(int i=0;i<size*2;i++){
                ID = idGen.getRandNum();
                Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
                satList2.insert(satellite);
                idList2.push_back(ID);
            }
            //calculates time for removing 2000 nodes
            start2 = clock();
            for(int i = 0; i < size*2; i++){
                satList2.remove(idList2[i]);
            }
            stop2 = clock();
            //calculates the ratio of time2 and time1
            time2 = stop2 - start2;
            time2 = time2/CLOCKS_PER_SEC;
            int ratio = time2/(2*time1);
            //returns if the ratio is in the range
            return ((1.1 - 0.4) <= ratio and (1.1 + 0.4) >= ratio);
        }
    
};

int main(){
    Tester test;
    if(test.insertNormal())
        cout << "Successful: Inserting in a Normal Case" << endl;
    else    
        cout << "Unsuccessful: Inserting in a Normal Case" << endl;
    if(test.insertEdge())
        cout << "Successful: Inserting in an Edge Case" << endl;
    else
        cout << "Unsuccessful: Inserting in an Edge Case" << endl;
    if(test.insertErrorRange())
        cout << "Successful: Did not insert ID out of Range" << endl;
    else
        cout << "Unsuccessful: Did not insert ID out of Range" << endl;
    if(test.insertMany())
        cout << "Successful: AVL and BST properties maintained after inserting many Sats" << endl;
    else
        cout << "Unsuccessful: AVL and BST properties maintained after inserting many Sats" << endl;
    if(test.removeNormal())
        cout << "Successful: Remove in a Normal Case" << endl;
    else
        cout << "Unsuccessful: Remove in a Normal Case" << endl;
    if(test.removeEdge())
        cout << "Successful: Remove in an Edge Case" << endl;
    else
        cout << "Unsuccessful: Remove in an Edge Case" << endl;
    if(test.removeMany())
        cout << "Successful: AVL and BST properties maintained after removing many Sats" << endl;
    else
        cout << "Unsuccessful: AVL and BST properties maintained after removing many Sats" << endl;
    if(test.checkRemoveDeorbited())
        cout << "Successful: Nodes with Deorbited State removed" << endl;
    else
        cout << "Unsuccessful: Nodes with Deorbited State removed" << endl;
    if(test.checkCountSatellites())
        cout << "Successful: Counted nodes with specified incline" << endl;
    else
        cout << "Unsuccessful: Counted nodes with specified incline" << endl;
    if(test.checkFindNormal())
        cout << "Successfil: Found node given ID" << endl;
    else
        cout << "Unsuccessful: Found node given ID" << endl;
    if(test.checkFindError())
        cout << "Successful: Node not found since doesn't exist" << endl;
    else
        cout << "Unsuccessful: Node not found since doesn't exist" << endl;
    if(test.assignmentNomal())
        cout << "Successful: Assignment for normal case" << endl;
    else
        cout << "Unsuccessful: Assignment for normal case" << endl;
    if(test.assignmentError())
        cout << "Successful: Assignment for error case" << endl;
    else
        cout << "Unsuccessful: Assignment for error case" << endl;
    if(test.timeMeasurementInsert())
        cout << "Successful: Insert ran in log n" << endl;
    else
        cout << "Unsuccessful: Insert running in log n" << endl;
    if(test.timeMeasurementRemove())
        cout << "Successful: Remove ran in log n" << endl;
    else
        cout << "Unsuccessful: Remove running in log n" << endl;
    
    return 0;
}