#include <iostream>
#include <random>
#include <queue>
#include <list>
#include <vector>
#include "event.h"
#include <algorithm>
#include<fstream>
#include "transmitter.h"
#include <cmath>
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
int max(int x , int y){
    if(x>y)return x;
    else return y;
}
int main() {
    double lambda;//arrival_rate
    double mu;//service_rate
    //double time = 0;
    mu=1;
    int MAXBUFFER;
    std::cout<<"Please enter lambda and MAX BUFFER: ";
    std::cin>>lambda>>MAXBUFFER;

    transmitter host(MAXBUFFER);//The transmitter object
    std::list<event> items;//The Global Event List
    //event init(time+nedt(lambda),true,1);
    //items.push_back(init);
    //time = init.time;
    int arrival_num = 0;//The number of arriving packet
    int departure_num = 0;//The number of departing packet

    /*event new_departure(0,false,0);//Declare before we use it. Actually it's virtual.
    for(int i = 0; i < 10;i++){
        if(i==3){
            int j=0;
        }
        time=time+nedt(lambda);//Time of the arrival event
        event new_arrival(time,true,i,host.queueLength(),host.getPacketLoss());


        if(!host.isBusy()){
            departure_time = time + nedt(mu);
            host.current_packet = i;
            //items.push_back(new_arrival);
            new_departure.set(departure_time,false,host.getCurrentPacket(),host.queueLength(),host.getPacketLoss());
        }//if the transmitter is not busy, just transmit the packet
        else if(!host.bufferIsFull()){
            if(host.current_packet!=new_arrival.number)host.buffer.push(new_arrival.number);
            //items.push_back(new_arrival);
        }// else if it's not full, enqueue the  packet
        else host.packet_loss++;//else packet loss happens

        if(time>departure_time && departure_time>0){
            items.push_back(new_departure);
            if(!host.buffer.empty()){
                host.current_packet = host.buffer.front();
                host.buffer.pop();
            }
            else host.current_packet=0;
            departure_time += nedt(mu);
            new_departure.set(departure_time,false,host.getCurrentPacket(),host.queueLength(),host.getPacketLoss());

            i--;
            continue;
        }//Only push departure event when all the arrival events before it are pushed so that the order of GEL is well maintained.
        if(!host.bufferIsFull())items.push_back(new_arrival);
    }*/
    //event cur_arrival(0,1,0);
    //event cur_departure(0,2,0);
    event cur_event(0,false,0);//(cur_arrival>cur_departure)?cur_departure:cur_arrival;
    double departure_time=0;
    double arrival_time=0;
    arrival_time += nedt(lambda);
    for(int i = 0; i < 150000; i++){

        //if(host.buffer.empty() && cur_event.type)departure_time=arrival_time+nedt(mu);
        //else if(host.isBusy())departure_time+=nedt(mu);
        //arrival_time += nedt(lambda);
        //if(!host.isBusy())departure_time=arrival_time+nedt(mu);

        if(!cur_event.type){
            if(!host.isBusy())departure_time = arrival_time + nedt(mu);
            else departure_time += nedt(mu);
        }//If the last event is departure, we only have to update the time of next departure and keep the arrival time unchanged.
        else {
            arrival_time += nedt(lambda);
        }//else we only update the arrival time


        if(arrival_time<departure_time && arrival_num<50000){
            arrival_num++;
            if(!host.bufferIsFull())cur_event.set(arrival_time,true,arrival_num,cur_event.queueLength+1,cur_event.totalPacketLoss);
            else cur_event.set(arrival_time,true,arrival_num,cur_event.queueLength,cur_event.totalPacketLoss+1);//Packet loss happens.
            items.push_back(cur_event);
            if(!host.isBusy()){
                host.current_packet = arrival_num;//If the server is not busy, it starts to transmit the packet
            }
            else if(!host.bufferIsFull())host.buffer.push(arrival_num);//else put the packet into buffer if buffer not full
        }
        else{
            departure_num++;

            cur_event.set(departure_time, false, host.current_packet, cur_event.queueLength - 1,cur_event.totalPacketLoss);
            items.push_back(cur_event);
            if(host.current_packet==50000)break;
            host.current_packet = 0;
            if(!host.buffer.empty()){
                host.current_packet = host.buffer.front();
                host.buffer.pop();//enqueue the buffer
            }
        }
        //If arrival time is less than departure time, it means the arrival event takes place before the departure event so we push the arrival event to GEL.
        if(departure_num == 50000)break;
    }

    for(auto & item : items){
        item.print();
    }

    std::string filePath ="projectData/lambda="+std::to_string(lambda).substr(0,3)+".txt";
    if(MAXBUFFER<100){
        filePath = "projectData/lambda="+std::to_string(lambda).substr(0,3)+",MAXBUFFER="+std::to_string(MAXBUFFER)+".txt";
    }
    std::ofstream out;
    out.open(filePath);
    if(!out.is_open())std::cout<<"file not open";
    out<<"time,type,number,queueLength,totalPacketLoss"<<std::endl;
    for(auto & item : items){
        out<<item.time<<","<<item.type<<","<<item.number<<","<<item.queueLength<<","<<item.totalPacketLoss<<std::endl;
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
