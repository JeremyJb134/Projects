/***********************************************************
 ** File:    Bomb.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/16/2023
 ** Section: 11
 **********************************************************/

 #include "Bomb.h"

 // Name: Bomb() - Default Constructor
  // Description: Creates a new Bomb Monkey
  // Preconditions: None
  // Postconditions: Can create a Bomb Monkey
Bomb::Bomb() : Monkey(){
    SetType("Bomb");
}
  // Name: Bomb(string type, int damage, int location)
  // Description: Creates a new Bomb Monkey
  // Preconditions: None
  // Postconditions: Can create a Bomb Monkey
Bomb::Bomb(string type, int damage, int location) : Monkey(type, damage, location){}
  // Name: ~Bomb - Virtual Destructor
  // Description: Makes sure everything in child class is deallocated
  // Preconditions: None
  // Postconditions: Everything dynamically allocated is deallocated
Bomb::~Bomb(){
    SetType("");
    SetDamage(0);
    SetLocation(0);
}
  // Name: Attack
  // Description: Iterates through all bloons. Hits all bloons in same location as bomb.
  //              Returns total bloon health or damage (whichever is less) for earnings (money)
  //              EXTRA CREDIT: Only monkey that can pop Lead Bloons.
  // Preconditions: Bloons must be in same location as bomb monkey
  // Postconditions: Damages each bloon and returns earnings (total pops)
int Bomb::Attack(vector<Bloon*> bloonList){
    int total = 0;
    //iterates through bloonList and calls pop on each bloon with same location as the monkey
    for(unsigned int i = 0; i < bloonList.size(); i++){
        if (bloonList[i]->GetLocation() == GetLocation()){
            cout << "The bomb shooter launches a bomb!" << endl;
            total += bloonList[i]->Pop(GetDamage());
        }
    }
    return total;
}