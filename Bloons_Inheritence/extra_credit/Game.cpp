/***********************************************************
 ** File:    Game.cpp
 ** Project: CMSC 202 - Project 4
 ** Author:  Jeremy Bruce
 ** Date     4/16/2023
 ** Section: 11
 ** 
 **********************************************************/

 #include "Game.h"

// Name: Game() - Default Constructor
  // Description: Creates a new Game and populates money, round, and health
  // Preconditions: None
  // Postconditions: Initializes all game variables to defaults (constants)
  // including rounds played
Game::Game(){
  m_curRound = START_ROUND;
  m_curMoney = START_MONEY;
  m_curLife = START_LIFE;
}
  // Name: ~Game
  // Description: Destructor
  // Preconditions: None
  // Postconditions: Deallocates anything dynamically allocated in Game
Game::~Game(){
  //iterates through both vectors and deletes every index
  for (unsigned int i = 0; i < m_bloons.size(); i++){
    delete m_bloons[i];
  }
  for (unsigned int j = 0; j < m_monkeys.size(); j++){
    delete m_monkeys[j];
  }
}
  // Name: PrintMap()
  // Description: For each location on the path, outputs the bloons and the monkeys (or none)
  // Precondition: None
  // Postcondition: None
void Game::PrintMap(){
  //loops for how many locations there are
  for (int i = 0; i < PATH_LENGTH; i++){
    int counter = 0;
    cout << endl << "**Location " << i+1 << "**" << endl;
    cout << "--Monkeys--" << endl;
    //if the size is zero, return 0
    if (int(m_monkeys.size()) == 0){
      cout << "None" << endl;
    }
    else{
      for (unsigned int j = 0; j < m_monkeys.size(); j++){
        //if the monkey location == index + 1, which is locations 1 2 3, increases counter
        if (m_monkeys[j]->GetLocation() == i+1){
          cout << counter+1 << ". ";
          //couts each monkey if it equals the location
          cout << *m_monkeys[j] << endl;
          counter++;
        }
      }
      //if counter is 0 then there are no monkeys at the location
      if (counter == 0)
        cout << "None" << endl;
    }
    int counter2 = 0;
    cout << "<<Bloons>>" << endl;
    //if bloon size is zero there are no bloons
    if (int(m_bloons.size()) == 0)
      cout << "None" << endl;
    else{
      for (unsigned int k = 0; k < m_bloons.size(); k++){
        //if the bloon location == index + 1, which is location 1 2 3, icnreases counter
        if (m_bloons[k]->GetLocation() == i+1){
          cout << "Bloon: ";
          cout << *m_bloons[k];
          counter2++;
        }
      }
      //if counter is 0 then there are no balloons at the location
      if (counter2 == 0)
        cout << "None" << endl;
    }
  }
}
  // Name: ImproveMonkey
  // Description: Asks the user which monkey they would like to improve
  //              Increases chosen monkey's damage by IMPROVE_VALUE above
  // Precondition: Must have monkeys in m_monkeys and have enough money to pay (COST_IMPROVE)
  // Postcondition: Monkey's damage is permanently improved
void Game::ImproveMonkey(){
  //no monkeys to improve if size is 0
  if (int(m_monkeys.size()) == 0)
    cout << "No Monkeys to Improve." << endl;
  else{
    //if money is less then cost, then you can't buy monkeys
    if (m_curMoney < COST_IMPROVE)
      cout << "Not Enough Money to Improve." << endl;
    else{
      int choice;
      int size = int(m_monkeys.size());
      //input validation for choosing monkeys
      do{
        cout << "Which monkey would you like to improve (1-" << size << ")" << endl;
        for (int i = 0; i < size; i++){
          cout << i+1 << ". " << *m_monkeys[i] << endl;
        }
        cin.clear();
        cin.ignore(256, '\n');
        cin >> choice;
      //if choice is > size then ask again
      }while(choice < 1 or choice > size);
      //lowers choice by 1 since you need index
      choice--;
      //sets monkeys damage to its current damage + improve value
      m_monkeys[choice]->SetDamage(m_monkeys[choice]->GetDamage() + IMPROVE_VALUE);
      cout << m_monkeys[choice]->GetType() << " in position " << 
      m_monkeys[choice]->GetLocation() << " improved!" << endl;
      //decrease money by cost
      m_curMoney = m_curMoney - COST_IMPROVE;
    }
  }
}
  // Name: BuyMonkey
  // Description: Asks the user which monkey they would like to buy
  //            Reduces m_curMoney and then calls PlaceMonkey (which creates and places new monkey)
  // Precondition: Must have enough money to pay (COST_DART, COST_BOOMERANG, or COST_BOMB)
  // Postcondition: Specified monkey is added a chosen location on the path
void Game::BuyMonkey(){
  int choice = 0;
  do{
    //input validation for cin
    cout << "What type of monkey would you like to buy?" << endl
    << "1. Dart Monkey" << endl << "2. Boomerang Monkey" << endl
    << "3. Bomb Monkey" << endl << "4. Cancel" << endl;
    cin.clear();
    cin.ignore(256, '\n');
    cin >> choice;
  } while(choice < 1 or choice > 4);
  switch(choice){
    //checks to see if you have enough money for each monkey
    //calls PlaceMonkey using the users input as parameter
    case 1:
      if (m_curMoney >= COST_DART){
        PlaceMonkey(choice);
        m_curMoney = m_curMoney - COST_DART;
      }
      else
        cout << "Not Enough Money for Dart Monkey" << endl;
      break;
    case 2:
      if (m_curMoney >= COST_BOOMERANG){
        PlaceMonkey(choice);
        m_curMoney = m_curMoney - COST_BOOMERANG;
      }
      else
        cout << "Not Enough Money for Boomerang Monkey" << endl;
      break;
    case 3:
      if (m_curMoney >= COST_BOMB){
        PlaceMonkey(choice);
        m_curMoney = m_curMoney - COST_BOMB;
      }
      else
        cout << "Not Enough Money for Bomb Monkey" << endl;
      break;
  }
}
  // Name: PlaceMonkey(int choice)
  // Description: Based on monkey purchased in BuyMonkey, asks user where to place new monkey
  //              Dynamically allocates new monkey and populates location and adds to m_monkey
  // Preconditions: Successfully purchased new monkey
  // Postconditions: New monkey is added to m_monkey at chosen location
void Game::PlaceMonkey(int choice){
  int location = 0;
  do{
    cout << "Where would you like to place the new monkey? (1-" << PATH_LENGTH << ")" << endl;
    cin.clear();
    cin.ignore(256, '\n');
    cin >> location;
    //input validation to make sure they place it at a location
  } while(location < 1 or location > PATH_LENGTH);
  //if 1 then dart, 2 boomerang, and 3 is bomb
  //creates a new monkey and pushes it into the monkey vector
  if (choice == 1){
    Monkey *newMonkey = new Dart("Dart", DAMAGE_DART, location);
    m_monkeys.push_back(newMonkey);
    cout << "New Dart monkey placed in location " << location << endl;
  }
  else if (choice == 2){
    Monkey *newMonkey = new Boomerang("Boomerang", DAMAGE_BOOM, location);
    m_monkeys.push_back(newMonkey);
    cout << "New Boomerang monkey placed in location " << location << endl;
  }
  else if (choice == 3){
    Monkey *newMonkey = new Bomb("Bomb", DAMAGE_BOMB, location);
    m_monkeys.push_back(newMonkey);
    cout << "New Bomb monkey placed in location " << location << endl;
  }
}
  // Name: StartGame()
  // Description: Welcomes the player to the game. Continually calls MainMenu until user quits
  // Preconditions: None
  // Postconditions: Thanks user when quitting
void Game::StartGame(){
  cout << "Welcome to UMBC Bloons!" << endl;
  int flag = 0;
  do{
    flag = MainMenu();
    //keeps calling main menu until it returns 0
  }while(flag != 6);
  //if the life is <=0 then cout they lost
  if (m_curLife <= 0)
    cout << "Sorry, you have lost the game." << endl;
  cout << "Thanks for playing!" << endl;
}
  // Name: MainMenu()
  // Description: Checks to see if player still has life. If m_curLife <= 0, game ends with loss.
  //              Display menu (View Map, Buy New Monkey, Improve Existing Monkey
  //              Progress Round, Check Stats, or Quit).
  //              Calls function related to user choice.
  // Preconditions: None
  // Postconditions: Game continues until quit or player runs out of life
int Game::MainMenu(){
  //if life is <= 0 then return 6 to end game
  if (m_curLife <= 0)
    return 6;
  int choice = 0;
  cout << "What would you like to do?" << endl << "1. View Map"
  << endl << "2. Buy New Monkey" << endl << "3. Improve Existing Monkey"
  << endl << "4. Progress Round" << endl << "5. Check Stats"
  << endl << "6. Quit" << endl;
  cin >> choice;
  //uses switch statement to call correct functions
  switch(choice){
    case 1:
      PrintMap();
      return choice;
      break;
    case 2:
      BuyMonkey();
      return choice;
      break;
    case 3:
      ImproveMonkey();
      return choice;
      break;
    case 4:
      PlayRound();
      return choice;
      break;
    case 5:
      Stats();
      return choice;
      break;
    case 6:
      return choice;
      break;
    default:
      return 0;
  }
}
  // Name: Stats()
  // Description: Displays the information about the game
  //              (current round, number of monkeys, money, and lives left)
  // Preconditions: None
  // Postconditions: None
void Game::Stats(){
  //couts all the member variables
  cout << "**CURRENT STATS**" << endl << "Current Round: " << m_curRound << endl
  << "Monkeys Working: " << int(m_monkeys.size()) << endl << "Current Money: " 
  << m_curMoney << endl << "Current Life: " << m_curLife << endl;
}
  // Name: PlayRound()
  // Description: Announces beginning and end of round
  //              Calls PopulateBloons once then ResolveBattle once.
  //              Calls RemovePopped (5 times due to erase function)
  //              Calls MoveBloons once then calls CheckPath (5 times due to erase call)
  //              Increments curRound
  // Preconditions: None
  // Postconditions: Round progresses
void Game::PlayRound(){
  cout << "Starting Round " << m_curRound << endl;
  //calls resolve to calculate how much earned, and populate for making new balloons
  PopulateBloons();
  ResolveBattle();
  for (int i = 0; i < 5; i++){
    RemovePopped();
  }
  //moves balloon to next location and checks to see if any bloon is at location 4
  MoveBloons();
  for (int j = 0; j < 5; j++){
    CheckPath();
  }
  cout << "The bloons move along the path" << endl;
  cout << "Round " << m_curRound << " Completed" << endl;
  m_curRound++;
}
  // Name: PopulateBloons
  // Description: Each round, dynamically allocated bloons added to path in position START_BLOON
  //              Bloons have a minimum of 1 health.
  //              For each round, the number of new bloons increases matching the round number
  //              For example: Round 1 = 1 red bloon (health 1)
  //              Round 2 = 1 red bloon (health 1) and 1 blue bloon (health 2)
  //              Round 3 = 1 red (health 1) and 1 blue (health 2) and 1 green (health 3)
  //              Round 4 = 1 red, 1 blue, 1 green, and 1 yellow (health 4) and so forth
  //              Notifies user that a new bloon (with color) has appeared.
  // Preconditions: Round is at least 1
  // Postconditions: All new bloons populated
void Game::PopulateBloons(){
  //creates how many bloons there are based on curRound
  for (int i = 0; i < m_curRound; i++){
    //sets health to the i+1 to make a new balloon from 1 to curRound
    Bloon *newBloon = new Basic(i+1, START_BLOON);
    cout << "A new " << newBloon->GetColor() << " bloon appears!" << endl;
    m_bloons.push_back(newBloon);
  }
  //starting from round 10, creates a new lead bloon every round
  if (m_curRound >= 10){
    Bloon *newLeadBloon = new Lead(HEALTH_LEAD, START_BLOON);
    cout << "A new " << newLeadBloon->GetColor() << " bloon appears!" << endl;
    m_bloons.push_back(newLeadBloon);
  }
  //starting from round 10, creates a new zebra bloon every round
  if (m_curRound >= 12){
    Bloon *newZebraBloon = new Zebra(HEALTH_ZEBRA, START_BLOON);
    cout << "A new " << newZebraBloon->GetColor() << " bloon appears!" << endl;
    m_bloons.push_back(newZebraBloon);
  }

}
  // Name: ResolveBattle()
  // Description: Each monkey attacks the whole bloon vector.
  //              For each pop, curMoney is increased.
  // Preconditions: Monkeys and bloons exist
  // Postconditions: Damage is dealt to bloons
void Game::ResolveBattle(){
  //iterates through monkey list and makes them all attack
  for (unsigned int i = 0; i < m_monkeys.size(); i++){
    //increaes money based on how much damage the monkey does
    m_curMoney += m_monkeys[i]->Attack(m_bloons);
  }
}
  // Name: RemovePopped()
  // Description: Iterates through m_bloons and if health is <= 0, bloon is deallocated and
  //              removed from m_bloons
  //         HINT: m_bloons.erase(m_bloons.begin() + counter) will erase at location
  // Preconditions: Monkeys and bloons exist
  // Postconditions: Bloons are removed
void Game::RemovePopped(){
  int counter = 0;
  int location = 0;
  //iterates through the vector to see how many bloons with health 0
  for (unsigned int i = 0; i < m_bloons.size(); i++){
    if (m_bloons[i]->GetHealth() <= 0)
      counter++;
  }
  //does the for loop for how many bloons with 0 health there are
  for (int j = 0; j < counter; j++){
    //iterates through the vector and gets the location each time it loops
    for (unsigned int i = 0; i < m_bloons.size(); i++){
      if (m_bloons[i]->GetHealth() <= 0)
        location = i;
    }
    //if the color is lead, make a new black bloon at the same location
    if (m_bloons[location]->GetColor() == LEAD_BLOON){
      Bloon *newBloon = new Basic(HEALTH_AFTER, m_bloons[location]->GetLocation());
      m_bloons.push_back(newBloon);
    }
    //if the color is zebra make 2 new black bloon at the location
    if (m_bloons[location]->GetColor() == ZEBRA_BLOON){
      Bloon *newBloon = new Basic(HEALTH_AFTER, m_bloons[location]->GetLocation());
      m_bloons.push_back(newBloon);
      Bloon *newBloon2 = new Basic(HEALTH_AFTER, m_bloons[location]->GetLocation());
      m_bloons.push_back(newBloon2);
    }
    //deletes and erase the bloons at the location
    delete m_bloons[location];
    m_bloons.erase(m_bloons.begin() + location);
  }
}
  // Name: MoveBloons
  // Description: For each bloon that still lives, it moves one location down the path
  // Preconditions: Monkeys and bloons exist
  // Postconditions: Bloons increment their location
void Game::MoveBloons(){
  for (unsigned int i = 0; i < m_bloons.size(); i++){
    //sets location to 1 if its location is the start, increases by 1 if its somewhere else
    if (m_bloons[i]->GetLocation() == START_BLOON)
      m_bloons[i]->SetLocation(1);
    else
      m_bloons[i]->SetLocation(m_bloons[i]->GetLocation() + 1);
  }
}
  // Name: CheckPath
  // Description: Iterates over m_bloons to see if location is equal to (or greater than)
  //              PATH_LENGTH. If location is equal to PATH_LENGTH, causes damage to player.
  //              Damage to player is one per health of bloon.
  //              If 10 green bloons (health 3) hit player, player takes 30 damage
  //              Bloons that hurt player are deallocated and erased from m_bloons
  //              HINT: m_bloons.erase(m_bloons.begin() + counter) will erase at location
  // Preconditions: Monkeys and bloons exist
  // Postconditions: Bloons are removed
void Game::CheckPath(){
  int counter = 0;
  int location = 0;
  //iterates through the vector to see how many bloons with location > 3
  for (unsigned int i = 0; i < m_bloons.size(); i++){
    if (m_bloons[i]->GetLocation() > PATH_LENGTH)
      counter++;
  }
  //does the for loop for how many bloons with location > 3 there are
  for (int j = 0; j < counter; j++){
    //iterates through the vector and finds a bloon with location > 3, each time it loops
    for (unsigned int i = 0; i < m_bloons.size(); i++){
      if (m_bloons[i]->GetLocation() > PATH_LENGTH)
        location = i;
    }
    //takes different amounts of damage if its lead and zebra balloons
    if (m_bloons[location]->GetColor() == LEAD_BLOON){
      cout << "A bloon made it to the end of the path and you took " << LEAD_DAMAGE << " damage." << endl;
      m_curLife -= LEAD_DAMAGE;
    }
    if (m_bloons[location]->GetColor() == ZEBRA_BLOON){
      cout << "A bloon made it to the end of the path and you took " << ZEBRA_DAMAGE << " damage." << endl;
      m_curLife -= ZEBRA_DAMAGE;
    }
    else{
      cout << "A bloon made it to the end of the path and you took " << m_bloons[location]->GetHealth() 
      << " damage." << endl;
      m_curLife -= m_bloons[location]->GetHealth();
    }
    
    //decrease life by bloons health and deletes the bloon at location
    delete m_bloons[location];
    m_bloons.erase(m_bloons.begin() + location);
  }
}