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
    int queueLength;//The queue length when an event takes place
    int totalPacketLoss;//The total packet loss when an event takes place

    event(double time, bool type, int number):time(time),type(type),number(number),queueLength(0),totalPacketLoss(0){}
    event(double time,bool type,int number,int queueLength,int totalPacketLoss):time(time),type(type),number(number),queueLength(queueLength),totalPacketLoss(totalPacketLoss){}
    friend bool operator > (const event& lhs,const event& rhs){return lhs.time > rhs.time;}
    void print(){std::cout<<"time="<<time<<", type="<<type<<", number="<<number<<std::endl;}
    void set(double Time,bool Type,int Number,int QueueLength,int TotalPacketLoss){
        time = Time;
        type = Type;
        number = Number;
        queueLength = QueueLength;
        totalPacketLoss = TotalPacketLoss;
    }

};
#endif //INC_152PJ1_EVENT_H
