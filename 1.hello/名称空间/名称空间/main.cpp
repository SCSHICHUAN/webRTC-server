//
//  main.cpp
//  名称空间
//
//  Created by Stan on 2021/7/1.
//

#include <iostream>
#include "Human.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    
    SC::Human *human = new SC::Human();
    human->setAge(33);
    std::cout<<human->getAge()<<std::endl;
    
    human->~Human();
    
    return 0;
}
