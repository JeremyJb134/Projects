// CMSC 341 - Fall 2023 - Project 4
#include "dealer.h"
CarDB::CarDB(int size, hash_fn hash, prob_t probing = DEFPOLCY){
    //makes sure size is in the range
    if(size < MINPRIME)
         m_currentCap = MINPRIME;
    else if(size > MAXPRIME)
        m_currentCap = MAXPRIME;
    //checks if size is prime, if not find next prime
    else{
        if(isPrime(size) == false)
            m_currentCap = findNextPrime(size);
        else
            m_currentCap = size;
    }
    m_hash = hash;
    m_currentSize = 0;
    //initalizes table array with empty cars
    m_currentTable = new Car[m_currentCap];
    for(int i = 0; i < m_currentCap; i++){
        m_currentTable[i] = EMPTY;
    }
    //initalizes all oldtable variables
    m_currNumDeleted = 0;
    m_currProbing = probing;
    m_oldTable = nullptr;
    m_oldCap = 0;
    m_oldSize = 0;
    m_oldNumDeleted = 0;
    m_newPolicy = NONE;
}

CarDB::~CarDB(){
    //deletes array
    if(m_currentTable != nullptr)
        delete[] m_currentTable;
    if(m_oldTable != nullptr)
        delete[] m_oldTable;
}

void CarDB::changeProbPolicy(prob_t policy){
    //sets new policy to policy passed
    m_newPolicy = policy;
}

bool CarDB::insert(Car car){
    //checks to make sure car is in range
    if(car.getDealer() < MINID or car.getDealer() > MAXID)
        return false;
    //gets index from hash function
    unsigned int index = m_hash(car.getModel()) % m_currentCap;
    int numProbs = 0;
    //if the index contains an object that isn't empty or deleted, then prob
    while(!m_currentTable[index].m_model.empty() and m_currentTable[index].m_used != false and numProbs <= m_currentSize){
        //if car is in there already then return false
        if(m_currentTable[index] == car)
            return false;
        //calculates next index using hash functions
        else if(m_currProbing == QUADRATIC){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * numProbs)) % m_currentCap;
        }
        else if(m_currProbing == DOUBLEHASH){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_currentCap;
        }        
        numProbs++;
    }
    //checks to make sure the index at the end of the loop is an available bucket to insert
    if(m_currentTable[index].m_model.empty() or !m_currentTable[index].m_used){
        m_currentTable[index] = car;
        m_currentSize++;
    }
    else 
        return false;
    //checks load factor or if the oldTable is initalized to start/continue rehashing
    float check = 0.5;
    if(lambda() > check or m_oldTable != nullptr){
        //if load factor is greater then 50, then set old table to current table, and reinitialize current table variables
        if(m_oldTable == nullptr){
            m_oldTable = m_currentTable;
            m_oldCap = m_currentCap;
            m_oldSize = (m_currentSize - m_currNumDeleted); //sets size only to number of live data
            m_oldProbing = m_currProbing;
            m_oldNumDeleted = 0;
            //if a new policy is wanted, then change policy to new policy
            if(m_newPolicy != NONE)
                m_currProbing = m_newPolicy;
            m_currentCap = findNextPrime(4 * (m_oldSize - m_currNumDeleted));
            m_currentTable = new Car[m_currentCap];
            m_currNumDeleted = 0;
            m_currentSize = 0;
        }
        //incrementally rehashes everytime we insert and the oldTable is not null
        rehash();
    }
    return true;
}

bool CarDB::remove(Car car){
    //gets index from hash function
    unsigned int index = m_hash(car.getModel()) % m_currentCap;
    int numProbs = 0;
    bool flag = false;
    //keeps probing until we reach an empty object or we go out of range of size
    while(!m_currentTable[index].m_model.empty() and numProbs <= m_currentSize and flag == false){
        //cout << "probing" << endl;
        //if the table contains the car, then set used to false, and return true
        if(m_currentTable[index] == car and m_currentTable[index].m_used != false){
            m_currentTable[index].m_used = false;
            m_currNumDeleted++;
            flag = true;
        }
        //probs to find next index
        if(m_currProbing == QUADRATIC){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * numProbs)) % m_currentCap;
        }
        else if(m_currProbing == DOUBLEHASH){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_currentCap;
        }        
        numProbs++;
    }
    if(m_oldTable != nullptr){
        index = m_hash(car.getModel()) % m_oldCap;
        numProbs = 0;
        //checks the old table for the car
        while(m_oldTable[index].m_model.empty() and numProbs <= m_oldSize){
            if(m_oldTable[index] == car and m_oldTable[index].m_used != false){
                m_oldTable[index].m_used = false;
                m_oldNumDeleted++;
                flag = true;
            }
            else if(m_oldProbing == QUADRATIC){
                index = (m_hash(car.getModel()) % m_oldCap + (numProbs * numProbs)) % m_oldCap;
            }
            else if(m_oldProbing == DOUBLEHASH){
                index = (m_hash(car.getModel()) % m_oldCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_oldCap;
            }        
            numProbs++;
        }
    }
    //if flag is false then car does not exist in either tables
    if(flag == false)
        return false;
    //checks if deleted ratio is greater then the 80% of the size
    float check = 0.8;
    if(deletedRatio() > check or m_oldTable != nullptr){
        if(m_oldTable == nullptr){
            m_oldTable = m_currentTable;
            m_oldCap = m_currentCap;
            m_oldSize = (m_currentSize - m_currNumDeleted); //sets size only to number of live data
            m_oldProbing = m_currProbing;
            m_oldNumDeleted = 0;
            if(m_newPolicy != NONE)
                m_currProbing = m_newPolicy;
            m_currentCap = findNextPrime(4 * (m_oldSize - m_currNumDeleted));
            m_currentTable = new Car[m_currentCap];
            m_currNumDeleted = 0;
            m_currentSize = 0;
        }
        //incrementally rehashes everytime we remove and the oldTable is not null
        rehash();
    }
    return true;
}

Car CarDB::getCar(string model, int dealer) const{
    unsigned int index = m_hash(model) % m_currentCap;
    int numProbs = 0;
    //checks current table for the car
    while(!m_currentTable[index].m_model.empty() and m_currentTable[index].m_used != false and numProbs <= m_currentSize){
        if(m_currentTable[index].m_model == model and m_currentTable[index].m_dealer == dealer){
            return m_currentTable[index];
        }
        else if(m_currProbing == QUADRATIC){
            index = (m_hash(model) % m_currentCap + (numProbs * numProbs)) % m_currentCap;
        }
        else if(m_currProbing == DOUBLEHASH){
            index = (m_hash(model) % m_currentCap + (numProbs * (11 - m_hash(model) % 11))) % m_currentCap;
        }        
        numProbs++;
    }
    Car emptyCar;
    if(m_oldTable == nullptr)
        return emptyCar;
    index = m_hash(model) % m_oldCap;
    numProbs = 0;
    //checks old table for the car
    while(!m_oldTable[index].m_model.empty() and m_oldTable[index].m_used != false and numProbs <= m_oldSize){
        if(m_oldTable[index].m_model == model and m_oldTable[index].m_dealer == dealer){
            return m_oldTable[index];
        }
        else if(m_oldProbing == QUADRATIC){
            index = (m_hash(model) % m_oldCap + (numProbs * numProbs)) % m_oldCap;
        }
        else if(m_oldProbing == DOUBLEHASH){
            index = (m_hash(model) % m_oldCap + (numProbs * (11 - m_hash(model) % 11))) % m_oldCap;
        }        
        numProbs++;
    }
    //returns an empty car if nothing was found
    return emptyCar;

}

float CarDB::lambda() const {
    //float division to find load factor
    return float(m_currentSize)/m_currentCap;
}

float CarDB::deletedRatio() const {
    //float division to find deleted ratio
    return float(m_currNumDeleted)/m_currentSize;
}

void CarDB::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool CarDB::updateQuantity(Car car, int quantity){
    unsigned int index = m_hash(car.getModel()) % m_currentCap;
    int numProbs = 0;
    //looks in current table to update car quantity
    while(!m_currentTable[index].m_model.empty() and m_currentTable[index].m_used != false and numProbs <= m_currentSize){
        if(m_currentTable[index] == car){
            m_currentTable[index].m_quantity = quantity;
            return true;
        }
        else if(m_currProbing == QUADRATIC){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * numProbs)) % m_currentCap;
        }
        else if(m_currProbing == DOUBLEHASH){
            index = (m_hash(car.getModel()) % m_currentCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_currentCap;
        }        
        numProbs++;
    }
    index = m_hash(car.getModel()) % m_currentCap;
    numProbs = 0;
    //looks in old table to update car quantity
    while(!m_oldTable[index].m_model.empty() and m_oldTable[index].m_used != false and numProbs <= m_oldSize){
        if(m_oldTable[index] == car){
            m_oldTable[index].m_quantity = quantity;
            return true;
        }
        else if(m_oldProbing == QUADRATIC){
            index = (m_hash(car.getModel()) % m_oldCap + (numProbs * numProbs)) % m_oldCap;
        }
        else if(m_oldProbing == DOUBLEHASH){
            index = (m_hash(car.getModel()) % m_oldCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_oldCap;
        }        
        numProbs++;
    }
    //returns false if no car was updated
    return false;
}

bool CarDB::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int CarDB::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Car &car ) {
    if (!car.m_model.empty())
        sout << car.m_model << " (" << car.m_dealer << "," << car.m_quantity<< ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Car& lhs, const Car& rhs){
    // since the uniqueness of an object is defined by model and delaer
    // the equality operator considers only those two criteria
    return ((lhs.m_model == rhs.m_model) && (lhs.m_dealer == rhs.m_dealer));
}

void CarDB::rehash(){
    //calculaes how much 1/4 is of the number of live data buckets
    int quarter = (m_oldSize)/4;
    int inserts = 0;
    int counter = 0;
    //stops inserting when the numbers of inserts equal 1/4
    while(inserts != quarter){
        //trys to insert a non empty or deleted object into the new table
        if(!m_oldTable[counter].m_model.empty() and m_oldTable[counter].m_used != false){
            Car car = m_oldTable[counter];
            unsigned int index = m_hash(car.getModel()) % m_currentCap;
            int numProbs = 0;
            //inserts car from old table into new table
            while(!m_currentTable[index].m_model.empty() and m_currentTable[index].m_used != false and numProbs <= m_currentSize){
                if(m_currProbing == QUADRATIC){
                    index = (m_hash(car.getModel()) % m_currentCap + (numProbs * numProbs)) % m_currentCap;
                }
                else if(m_currProbing == DOUBLEHASH){
                    index = (m_hash(car.getModel()) % m_currentCap + (numProbs * (11 - m_hash(car.getModel()) % 11))) % m_currentCap;
                }        
                numProbs++;
            }
            if(m_currentTable[index].m_model.empty() or !m_currentTable[index].m_used){
                m_currentTable[index] = car;
                m_oldTable[counter].m_used = false;
                m_oldNumDeleted++;
                m_currentSize++;
                inserts++;
            }
            //deletes oldTable if the number of deleted equals the size of old table
            if(m_oldNumDeleted == m_oldSize){
                delete[] m_oldTable;
                m_oldTable = nullptr;
                m_oldCap = 0;
                m_oldSize = 0;
                m_oldNumDeleted = 0;
                m_oldProbing = NONE;
                inserts = quarter;
            }
        }   
        counter++;
    }
}   