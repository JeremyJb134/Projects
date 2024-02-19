/***********************************************************
 ** File:    Basic.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/12/2023
 ** Section: 11
 **********************************************************/

 #include "Basic.h"

 // Name: Basic() - Default Constructor
  // Description: Creates a new Basic Bloon
  // Preconditions: None
  // Postconditions: Can create a Basic Bloon
Basic::Basic() : Bloon(){
    m_color = "color";
}
  // Name: Basic(int health, int location)
  // Description: Creates a new basic bloon with health at a specific location
  // if health = 1, red, 2=blue, 3=green, 4=yellow, 5=pink, >=6=black
  // Preconditions: None
  // Postconditions: Can create a basic bloon
Basic::Basic(int health, int location) : Bloon(health, location){
    //uses switch statement to determine the color, health - 1 because enum starts at 0
    switch(health - 1){ 
        case red:
            m_color = "red";
            break;
        case blue:
            m_color = "blue";
            break;
        case green:
            m_color = "green";
            break;
        case yellow:
            m_color = "yellow";
            break;
        case pink:
            m_color = "pink";
            break;
        default:
            m_color = "black";
            break;
    }
}
  // Name: ~Basic - Virtual Destructor
  // Description: Makes sure everything in child class is deallocated
  // Preconditions: None
  // Postconditions: Everything dynamically allocated is deallocated
Basic::~Basic(){}
  // Name: Pop
  // Description: Changes the color of the bloon based on how many health it has left
  // red = 1, blue = 2, green = 3, yellow = 4, pink = 5, black >=6
  // Preconditions: None
  // Postconditions: Returns number of pops that occurred.
  // Number of pops is the lesser of (health left of bloon) or (damage caused by monkey).
int Basic::Pop(int damage){
    //if health is 0, then no damage is done
    if (GetHealth() == 0)
        return 0;
    else if (damage >= GetHealth()){
        //damage >= health just return damage 
        cout << "The " << GetColor() << " balloon pops!" << endl;
        //sets damage to health so you can set health to 0
        damage = GetHealth();
        SetHealth(0);
        cout << "The bloon is now gone!" << endl;
        return damage;
    }
    else{
        cout << "The " << GetColor() << " balloon pops!" << endl;
        int health = GetHealth() - damage;
        //updates health to be health - damage, updates the color based on new health
        SetHealth(health);
        switch(health - 1){
            case red:
                m_color = "red";
                break;
            case blue:
                m_color = "blue";
                break;
            case green:
                m_color = "green";
                break;
            case yellow:
                m_color = "yellow";
                break;
            case pink:
                m_color = "pink";
                break;
            default:
                m_color = "black";
                break;
        }
        cout << "The bloon is now " << m_color << "." << endl;
        return damage;
    }
    return 0;
}
  // Name: GetColor()
  // Description: Returns the current color of a bloon
  // Preconditions: None
  // Postconditions: None
string Basic::GetColor(){
    return m_color;
}