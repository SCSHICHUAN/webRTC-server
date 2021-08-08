//
//  main.cpp
//  多态
//
//  Created by Stan on 2021/7/1.
//

#include <iostream>
#include "Move.hpp"
#include "Human.hpp"
#include "Cat.hpp"


using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    Move *move;
    
    
    move = new Cat();
    move->move();
    
    move = new Human();
    move->move();
    
    move = new Move;
    move->move();
    

    return 0;
}
