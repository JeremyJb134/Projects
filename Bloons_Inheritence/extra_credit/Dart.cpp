/***********************************************************
 ** File:    Dart.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/16/2023
 ** Section: 11
 ** This file contains the header file of the Dart class
 **********************************************************/

 #include "Dart.h"
// Name: Dart() - Default Constructor
  // Description: Creates a new Dart Monkey
  // Preconditions: None
  // Postconditions: Can create a Dart Monkey
Dart::Dart() : Monkey() {
    SetType("Dart");
}
// Name: Dart(string type, int damage, int location)
  // Description: Creates a new Dart Monkey
  // Preconditions: None
  // Postconditions: Can create a Dart Monkey
Dart::Dart(string type, int damage, int location) : Monkey(type, damage, location){}
  // Name: ~Dart - Virtual Destructor
  // Description: Makes sure everything in child class is deallocated
  // Preconditions: None
  // Postconditions: Everything dynamically allocated is deallocated
Dart::~Dart(){
    SetDamage(0);
    SetType("");
    SetLocation(0);
}
  // Name: Attack
  // Description: Iterates through all bloons. Hits first bloon in the same location as dart.
  //              Returns total bloon health or damage (whichever is less) for earnings (money)
  // Preconditions: Bloon must be in same location as Dart monkey
  // Postconditions: Damages one bloon and returns earnings (total pops)
int Dart::Attack(vector<Bloon*> bloonList){
    //returns the damage of the first hit to the first balloon with same location as monkey
    for (unsigned int i = 0; i < bloonList.size(); i++){
        //makes sure to not hit lead bloons
        if (bloonList[i]->GetLocation() == GetLocation() and bloonList[i]->GetColor() != "lead"){
            cout << "The dart monkey throws a dart!" << endl;
            return bloonList[i]->Pop(GetDamage());
        }
    }
    return 0;
}


