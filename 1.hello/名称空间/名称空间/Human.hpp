//
//  Human.hpp
//  名称空间
//
//  Created by Stan on 2021/7/1.
//

#ifndef Human_hpp
#define Human_hpp

#include <iostream>

namespace SC {

class Human{
    
public:
    Human(){
        std::cout<<"constructor"<<std::endl;
        age = 30;
    };
    ~Human(){
        std::cout<<"销毁"<<std::endl;
    }
public:
    void setAge(int a);
    int  getAge();
private:
    int age;
};

}



#endif /* Human_hpp */
