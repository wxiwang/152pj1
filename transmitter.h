//
// Created by a cat on 2020/4/26.
//

#ifndef INC_152PJ1_TRANSMITTER_H
#define INC_152PJ1_TRANSMITTER_H

#include <queue>
class transmitter{
public:
    bool isBusy(){return current_packet;}
    int queueLength(){return buffer.size()+1;}
    int getCurrentPacket(){return current_packet;}
    int getPacketLoss(){return packet_loss;}
    transmitter()= default;
    bool bufferIsFull(){return buffer.size()>=MAXBUFFER;}
    std::queue<int> buffer;//The integers in it are the numbers of packets.

    int current_packet=0;//The integer is the number of the packet that is being transmitted. It's 0 if the server is idle.
    int packet_loss=0;//Total packet loss
    int MAXBUFFER=100000;
};
#endif //INC_152PJ1_TRANSMITTER_H
