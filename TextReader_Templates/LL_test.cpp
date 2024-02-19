#include <iostream>
#include "LL.cpp"
using namespace std;

//Comment out any tests that you don't want to run
//Then run make LL_test then make valLL

int main () {
  //Test 1 - Default Constructor and Push
  cout << "Test 1 - Default Constructor and Inserts" << endl;
  //Test Default Constructor
  LL <string>* newLL1 = new LL<string>();
  //Push 4 nodes into LL
  newLL1->Insert("candy");
  newLL1->Insert("cookies");
  newLL1->Insert("candy");
  newLL1->Insert("bananas");
  newLL1->Insert("dogs");
  newLL1->Insert("apples");
  newLL1->Insert("elephants");
  newLL1->Insert("barf");
  newLL1->Insert("candy");
  newLL1->Insert("cookies");
  newLL1->Insert("candy");
  newLL1->Insert("bananas");
  newLL1->Insert("dogs");
  newLL1->Insert("apples");
  newLL1->Insert("elephants");
  newLL1->Insert("barf");
  newLL1->Display();

  //  delete newLL1;

  //Test 2 - Copy Constructor
  cout << "Test 2 - Copy Constructor" << endl;
  //Test Copy constructor
  LL <string>* newLL2 = new LL<string>(*newLL1);
  cout << "*******Original*********" << endl;
  newLL1->Display();
  cout << "*******Copy*********" << endl;
  newLL2->Display();
  //delete newLL1;
  //delete newLL2;
  
  //Test 3 - Overloaded Assignment Operator
  cout << "Test 3 - Overloaded Assignment Operator" << endl;
  LL <string>* newLL3 = new LL<string>();
  cout << "Doing assignment" << endl;
  *newLL3 = *newLL1;
  cout << "*******Assignment*********" << endl;
  newLL3->Display();
  cout << endl;
  //  delete newLL1;
  //delete newLL2;
  //delete newLL3;

  //Test 4 - Test Display and Overloaded <<
  cout << "Test 4 - Display and Overloaded << Running" << endl;
  cout << "newLL1 Display Function" << endl;
  newLL1->Display();
  cout << "newLL1 Overloaded" << endl;
  cout << *newLL1;
  cout << "RemoveAt(candy)" << endl;
  newLL1->RemoveAt("candy");
  cout << "newLL1 Display Function" << endl;
  newLL1->Display();
  delete newLL1;
  delete newLL2;
  delete newLL3;
  return 0;
}
