//
//  Cat.hpp
//  多态
//
//  Created by Stan on 2021/7/1.
//

#ifndef Cat_hpp
#define Cat_hpp

#include <stdio.h>
#include "Move.hpp"

class Cat:public Move{
    
public:
    virtual void move();
};

#endif /* Cat_hpp */
