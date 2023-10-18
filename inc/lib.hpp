#pragma once

#include <Resources.hpp>
#include <iostream>

namespace lib {
    void test();

    class myClass {
        private: 
            myClass(){
                std::cout << "Hello from myClass!\n";
                std::cout << Resources::text::lorem_ipsum1 << std::endl;
            };
        public:
            static myClass& getInstance(){
                static myClass instance;
                return instance;
            }
    };
}