/***********************************************************
 ** File:    Boomerang.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/16/2023
 ** Section: 11
 ** 
 **********************************************************/

 #include "Boomerang.h"

 // Name: Boomerang() - Default Constructor
  // Description: Creates a new Boomerang Monkey
  // Preconditions: None
  // Postconditions: Can create a Boomerang Monkey
Boomerang::Boomerang() : Monkey(){
    SetType("Boomerang");
}
  // Name: Boomerang(string type, int damage, int location)
  // Description: Creates a new Boomerang Monkey
  // Preconditions: None
  // Postconditions: Can create a Boomerang Monkey
Boomerang::Boomerang(string type, int damage, int location) : Monkey(type, damage, location){}
  // Name: ~Boomerang - Virtual Destructor
  // Description: Makes sure everything in child class is deallocated
  // Preconditions: None
  // Postconditions: Everything dynamically allocated is deallocated
Boomerang::~Boomerang(){
    SetType("");
    SetDamage(0);
    SetLocation(0);
}
  // Name: Attack
  // Description: Iterates through all bloons. For each bloon in the same location as boomerang,
  //              hits each bloon twice (once out and once back)
  //              Returns total bloon health or damage (whichever is less) for earnings (money)
  // Preconditions: Bloons must be in same location as Boomerang Monkey
  // Postconditions: Damages each bloon and returns earnings (total pops)
int Boomerang::Attack(vector<Bloon*> bloonList){
    int total = 0;
    //iterates through bloonList and calls pop on each bloon with same location as the monkey
    //calls pop twice since boomerang hits twice
    for(unsigned int i = 0; i < bloonList.size(); i++){
        //makes sure to not hit lead bloons
        if (bloonList[i]->GetLocation() == GetLocation() and bloonList[i]->GetColor() != "lead"){
            cout << "The boomerang monkey throws a boomerang!" << endl;
            total += bloonList[i]->Pop(GetDamage()) + bloonList[i]->Pop(GetDamage());
        }
    }
    return total;
}