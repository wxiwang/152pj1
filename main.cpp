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

double nedt(double rate){
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
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

    int arrival_num = 0;//The number of arriving packet
    int departure_num = 0;//The number of departing packet


    event cur_event(0,false,0);//(cur_arrival>cur_departure)?cur_departure:cur_arrival;
    double departure_time=0;
    double arrival_time=0;
    arrival_time += nedt(lambda);
    for(int i = 0; i < 150000; i++){

        if(!cur_event.type){
            if(!host.isBusy())departure_time = arrival_time + nedt(mu);
            else departure_time += nedt(mu);
        }//If the last event is departure, we only have to update the time of next departure and keep the arrival time unchanged.
        else {
            arrival_time += nedt(lambda);
        }//else we only update the arrival time

        //If arrival time is less than departure time, it means the arrival event takes place before the departure event so we push the arrival event to GEL.
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
            if(host.current_packet==50000)break;//In the experiment we simulate transmitting 50000 packets.
            host.current_packet = 0;//server completes transmitting
            if(!host.buffer.empty()){
                host.current_packet = host.buffer.front();
                host.buffer.pop();//enqueue the buffer
            }
        }
        if(departure_num == 50000)break;
    }

    for(auto & item : items){
        item.print();//print out event information to the screen
    }

    //write to file
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
    return 0;
}
