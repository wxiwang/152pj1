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
    std::vector<event> items;
    //event init(time+nedt(lambda),true,1);
    //items.push_back(init);
    //time = init.time;
    //int arrival_num = 1;
    //int departure_number=1;
    double departure_time=0;
    event new_departure(0,false,0);//Declare before we use it. Actually it's virtual.
    for(int i = 0; i < 10;i++){
        time=time+nedt(lambda);//Time of the arrival event
        event new_arrival(time,true,i,host.queueLength(),host.getPacketLoss());

        if(!host.isBusy()){
            departure_time = time + nedt(mu);
            host.current_packet = i;
            new_departure.set(departure_time,false,host.getCurrentPacket(),host.queueLength(),host.getPacketLoss());
        }//if the transmitter is not busy, just transmit the packet
        else if(!host.bufferIsFull()){
            host.buffer.push(new_arrival.number);
            items.push_back(new_arrival);
        }// else if it's not full, enqueue the  packet
        else host.packet_loss++;//else packet loss happens

        if(time>departure_time){
            items.push_back(new_departure);
            host.current_packet = host.buffer.front();
            host.buffer.pop();
            departure_time += nedt(mu);
            new_departure.set(departure_time,false,host.getCurrentPacket(),host.queueLength(),host.getPacketLoss());

        }//Only push departure event when all the arrival events before it are pushed so that the order of GEL is well maintained.

    }
    /*std::priority_queue<event,std::vector<event>,std::greater<> > items;
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
    }*/
    return 0;
}
