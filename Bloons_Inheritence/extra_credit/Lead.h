/***********************************************************
 ** File:    Zebra.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/12/2023
 ** Section: 11
 **********************************************************/

#ifndef LEAD_H_
#define LEAD_H_

#include "Bloon.h"

class Lead: public Bloon {
 public:
  // Name: Basic() - Default Constructor
  // Description: Creates a new Basic Bloon
  // Preconditions: None
  // Postconditions: Can create a Basic Bloon
  Lead();
  // Name: Basic(int health, int location)
  // Description: Creates a new basic bloon with health at a specific location
  // if health = 1, red, 2=blue, 3=green, 4=yellow, 5=pink, >=6=black
  // Preconditions: None
  // Postconditions: Can create a basic bloon
  Lead(int, int);
  // Name: ~Basic - Virtual Destructor
  // Description: Makes sure everything in child class is deallocated
  // Preconditions: None
  // Postconditions: Everything dynamically allocated is deallocated
  ~Lead();
  // Name: Pop
  // Description: Changes the color of the bloon based on how many health it has left
  // red = 1, blue = 2, green = 3, yellow = 4, pink = 5, black >=6
  // Preconditions: None
  // Postconditions: Returns number of pops that occurred.
  // Number of pops is the lesser of (health left of bloon) or (damage caused by monkey).
  int Pop(int damage);
  // Name: GetColor()
  // Description: Returns the current color of a bloon
  // Preconditions: None
  // Postconditions: None
  string GetColor();
private:
  string m_color;
};
#endif