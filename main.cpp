#include <iostream>
#include <random>
#include <queue>
#include <list>
#include <vector>
#include "event.h"
#include <algorithm>
struct cmp{
    bool operator ()(const event& lhs,const event& rhs){
        return lhs.time < rhs.time;
    }
};
double nedt(double rate){
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}
int main() {
    double lambda;//arrival_rate
    double mu;//service_rate
    double time = 0;

    lambda=0.4;
    mu=0.5;
    /*std::vector<event> items;
    std::make_heap(items.begin(),items.end());
    event e1(3,true,1);
    event e2(5,true,2);
    items.push_back(e1);
    std::push_heap(items.begin(),items.end());
    items.push_back(e2);
    std::push_heap(items.begin(),items.end());
    std::cout << "The maximum element of heap after push is : "<<items.front().time<<std::endl;
    std::pop_heap(items.begin(),items.end());
    items.pop_back();
    std::cout << "The maximum element of heap after pop is : "<<items.front().time<<std::endl;
    std::cout << "Hello, World!" << std::endl;*/

    std::priority_queue<event,std::vector<event>,std::greater<> > items;
    event init(time+nedt(lambda),true,1);
    items.push(init);

    int num = 1;

    for(int i = 0; i < 10; i++){
        event currentEvent = items.top();
        items.pop();
        currentEvent.print();
        time = currentEvent.time;
        if(currentEvent.type){
            event nextEvent(time+nedt(mu),false,currentEvent.number);
            items.push(nextEvent);
        }
        else{
            num+=1;
            event arrivalEvent(time+nedt(lambda),true,num);
            items.push(arrivalEvent);
        }
    }
    return 0;
}
