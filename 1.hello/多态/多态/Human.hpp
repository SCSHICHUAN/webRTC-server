//
//  Human.hpp
//  多态
//
//  Created by Stan on 2021/7/1.
//

#ifndef Human_hpp
#define Human_hpp
#include "Move.hpp"

#include <iostream>
class Human:public Move{
public:
   virtual void move()
    {
        std::cout<<"行走"<<std::endl;
    }
};
#endif /* Human_hpp */
