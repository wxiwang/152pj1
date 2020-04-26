//
// Created by a cat on 2020/4/25.
//

#ifndef INC_152PJ1_EVENT_H
#define INC_152PJ1_EVENT_H

#include <iostream>
class event{
public:
    double time;//The time it take place
    bool type;//true if it's an arrival event otherwise it's departure
    int number;//# of the event
    event(double time, bool type, int number):time(time),type(type),number(number){}
    friend bool operator > (const event& lhs,const event& rhs){return lhs.time > rhs.time;}
    void print(){std::cout<<"time="<<time<<", type="<<type<<", number="<<number<<std::endl;}
};
#endif //INC_152PJ1_EVENT_H
