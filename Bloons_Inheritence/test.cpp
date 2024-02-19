#include <iostream>
#include <string>
using namespace std;

int main() {
    enum color{red, blue, green, yellow, pink, black};
    int health = 3;
    string m_color;
    switch(health){
        case 1:
            m_color = "Red";
            break;
        case 2:
            m_color = "Blue";
            break;
        case 3:
            m_color = "Green";
            break;
        case 4:
            m_color = "Yellow";
            break;
        case 5:
            m_color = "Pink";
            break;
        default:
            m_color = "Black";
            break;
    }
    cout << m_color << endl;
    return 0;
}


