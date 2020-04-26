#include <iostream>
#include <random>
#include <queue>
#include <list>
#include <vector>
#include "event.h"
#include <algorithm>
#include "transmitter.h"
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

    transmitter host;

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
