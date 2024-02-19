// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet(){
    m_root = nullptr;
}

SatNet::~SatNet(){
    postOrder(m_root);
}
void SatNet::insert(const Sat& satellite){
    if(satellite.getID() >= MINID and satellite.getID() <= MAXID){
        //updates height then checks for imbalances, then updates height again
        insertRecur(satellite, m_root);
        updateHeight(m_root);
        checkImbalance(m_root);
        updateHeight(m_root);

    }
}

void SatNet::clear(){
    postOrder(m_root);
}

void SatNet::remove(int id){
    if(findSatellite(id))
        m_root = removeRecur(m_root, id);
    //updates height then checks for imbalances, then updates height again
    updateHeight(m_root);
    checkImbalance(m_root);
    updateHeight(m_root);
}

void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {
    inOrder(m_root);
}

bool SatNet::setState(int id, STATE state){
    return setStateRecur(id, state, m_root);
}

void SatNet::removeDeorbited(){
    while(countSatellitesDeorbited(m_root) != 0){
        removeDeorbitedRecur(m_root);
    }
}

bool SatNet::findSatellite(int id) const {
    return findSatelliteRecur(m_root, id);
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    if(this != &rhs){
        if(rhs.m_root == nullptr)
            clear();
        else{
            clear();
            preOrder(m_root, rhs.m_root);
        }
    }
    return *this;
}

int SatNet::countSatellites(INCLIN degree) const{
    return countSatellitesRecur(degree, m_root);
}

void SatNet::postOrder(Sat* satellite){
    //uses postOrder to delete nodes starting from children
    if(satellite != nullptr){
        postOrder(satellite->getLeft());
        postOrder(satellite->getRight());
        delete satellite;
        satellite = nullptr;
    }
}

void SatNet::insertRecur(const Sat& satellite, Sat*& sat){
    if(sat == nullptr){
        //if null, then create new node and set pointer to point to node
        Sat* temp = new Sat(satellite.getID(), satellite.getAlt(), satellite.getInclin(), satellite.getState());
        sat = temp;
    }
    //goes left if less than, right if greater
    else if(satellite.getID() < sat->getID()){
        insertRecur(satellite, sat->m_left);
    }
    else if(satellite.getID() > sat->getID()){
        insertRecur(satellite, sat->m_right);
    }
}

void SatNet::updateHeight(Sat* sat){
    if(sat != nullptr){
        //updates heights starting from the leaves
        updateHeight(sat->getLeft());
        updateHeight(sat->getRight());
        //sets height to -1 for poiners that point to null
        int left = -1;
        int right = -1;
        if(sat->getLeft() != nullptr)
            left = sat->getLeft()->getHeight();
        if(sat->getRight() != nullptr)
            right = sat->getRight()->getHeight();
        //sets the height to highest height between left and right child + 1, since the parent is one higher than child
        int maxHeight = (right > left) ? right : left;
        int satHeight = 1 + maxHeight;
        sat->setHeight(satHeight);
    }
}

void SatNet::checkImbalance(Sat*& sat){
    if(sat != nullptr){
        //calls from the bottom nodes since those are the most recently inserted
        checkImbalance(sat->m_left);
        checkImbalance(sat->m_right);
        //sets height to -1 for poiners that point to null
        int left = -1;
        int right = -1;
        //gets height of left and right if not null
        if(sat->getLeft() != nullptr)
            left = sat->getLeft()->getHeight();
        if(sat->getRight() != nullptr)
            right = sat->getRight()->getHeight();
        if(right - left < -1){
            //if right is less than left, then do right roation
            left = -1;
            right = -1;
            //checks if left child's left child or right child is larger
            if(sat->getLeft()->getLeft() != nullptr)
                left = sat->getLeft()->getLeft()->getHeight();
            if(sat->getLeft()->getRight() != nullptr)
                right = sat->getLeft()->getRight()->getHeight();
            if(left > right)
                rightRotation(sat);
            else{
                leftRotation(sat->m_left);
                rightRotation(sat);
            }
        }
        else if(right - left > 1){
            //if right is greater than left, then do left roation
            left = -1;
            right = -1;
            if(sat->getRight()->getLeft() != nullptr)
                left = sat->getRight()->getLeft()->getHeight();
            if(sat->getRight()->getRight() != nullptr)
                right = sat->getRight()->getRight()->getHeight();
            //checks if right child's left child or right child is larger
            if(right > left)
                leftRotation(sat);
            else{
                rightRotation(sat->m_right);
                leftRotation(sat);
            }
        }
    }
}

void SatNet::rightRotation(Sat*& sat){
    //sets left child's right to the node we are rotating
    Sat* cur = sat;
    Sat* left = cur->getLeft();
    cur->setLeft(left->getRight());
    //sets x right child to sat 
    left->setRight(cur);
    //updates the pointer to x
    sat = left;
    updateHeight(m_root);
}

void SatNet::leftRotation(Sat*& sat){
    //sets right child's left to the node we are rotating
    Sat* cur = sat;
    Sat* right = cur->getRight();
    cur->setRight(right->getLeft());
    //sets y left child to sat 
    right->setLeft(cur);
    //updates the pointer to y
    sat = right;
    updateHeight(m_root);
}

Sat* SatNet::removeRecur(Sat*& aSat, int& id){
    if(aSat == nullptr) 
        return aSat;
    //id smaller so move left
    else if(aSat->m_id > id){
        aSat->m_left = removeRecur(aSat->m_left, id);
    }
    //id bigger so go right
    else if(aSat->m_id < id){
        aSat->m_right = removeRecur(aSat->m_right, id);
    }
    //id should equal
    //find min to replace node with smallest node to the right
    else if(aSat->m_left != nullptr and aSat->m_right != nullptr){
        Sat * temp = findMin(aSat->m_right);
        aSat->m_id = temp->m_id;
        aSat->m_state = temp->getState();
        aSat->m_inclin = temp->getInclin();
        aSat->m_altitude = temp->getAlt();
        aSat->m_right = removeRecur(aSat->m_right, temp->m_id);
    }
    //if one child go to that child, if not then delete node
    else{
        Sat * temp = aSat;
        if(aSat->m_right == nullptr){
            aSat = aSat->m_left;
        }
        else{
            aSat = aSat->m_right;
        }
        delete temp;

    }
    return aSat;
}
void SatNet::inOrder(Sat* satellite) const{
    //should be in ascending order, so inOrder traversal, left, root, right
    if(satellite != nullptr){
        inOrder(satellite->m_left);
        cout << satellite->m_id << ": " << satellite->getStateStr() << ": " << satellite->getInclinStr() << ": " 
        << satellite->getAltStr() <<  endl;
        inOrder(satellite->m_right);
    }
}

Sat* SatNet::findMin(Sat* aSat){
    //keeps going left until you get to leftmost node
    if(aSat == nullptr or aSat->m_left == nullptr){
        return aSat;
    }
    else{
        return findMin(aSat->m_left);
    }
}

bool SatNet::setStateRecur(int id, STATE state, Sat* satellite){
    if(satellite != nullptr){
    //if not nullptr check if id matches and set state if it does
        if(satellite->getID() == id){
            satellite->setState(state);
            return true;
        }
        //go right if greater, left if less
        else if(id > satellite->getID())
            return setStateRecur(id, state, satellite->m_right);
        else
            return setStateRecur(id, state, satellite->m_left);
    }
    //returns false if we reach nullptr
    return false;
}

void SatNet::removeDeorbitedRecur(Sat* satellite){
    if(satellite != nullptr){
        //traverses through entire tree
        removeDeorbitedRecur(satellite->m_left);
        removeDeorbitedRecur(satellite->m_right);
        //calls remove if state is deorbited
        if(satellite->getState() == 1){
           remove(satellite->getID());
        }
    }
}

int SatNet::countSatellitesDeorbited(Sat* satellite){
    if(satellite != nullptr){
        int count = 0;
        //traverse through entire tree
        if(satellite->getState() == 1){
            count += 1;
        }
        count += countSatellitesDeorbited(satellite->m_right);
        count += countSatellitesDeorbited(satellite->m_left);
        return count;
    }
    return 0;
}

int SatNet::countSatellitesRecur(INCLIN degree, Sat* satellite) const{
    if(satellite != nullptr){
        int count = 0;
        //traverse through entire tree
        if(satellite->getInclin() == degree){
            count += 1;
        }
        count += countSatellitesRecur(degree, satellite->m_right);
        count += countSatellitesRecur(degree, satellite->m_left);
        return count;
    }
    return 0;
}

bool SatNet::findSatelliteRecur(Sat* satellite, int id) const{
    if(satellite != nullptr){
        if(satellite->m_id == id)
            return true;
        //go right if greater, left if less
        else if(id > satellite->getID())
            return findSatelliteRecur(satellite->m_right, id);
        else
            return findSatelliteRecur(satellite->m_left, id);
    }
    //returns false if we reach nullptr
    return false;
}

void SatNet::preOrder(Sat*& aSat, Sat* rhs){
    //keeps going until rhs is null
    if(rhs != nullptr){
        //copies rhs data to new Sat object
        Sat* newSat = new Sat(rhs->m_id, rhs->m_altitude, rhs->m_inclin, rhs->m_state);
        newSat->setHeight(rhs->m_height);
        //sets aSat to point to new Sat object
        aSat = newSat;
        //moves left and right
        preOrder(aSat->m_left, rhs->m_left);
        preOrder(aSat->m_right, rhs->m_right);
    }
}