//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "computer.h"
#include "SimpleMessage_m.h"
#include<string.h>
#include<stdio.h>

Define_Module(Computer);

void Computer::initialize()
{
      numSent     = 0;
      numReceived = 0;
      hopCountStats.setName("hopCountStats");
      hopCountStats.setRange(0, 10);
      hopCountVector.setName("HopCount");
      WATCH(numSent);
      WATCH(numReceived);

    // Module 0 sends the first message
    if (getIndex() == 0) {
        // Boot the process scheduling the initial message as a self-message.
        SimpleMessage *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *ttmsg = check_and_cast<SimpleMessage *>(msg);
    if (ttmsg->getDestination() == getIndex()) {
            int hopcount = ttmsg->getHopCount();
        // Message arrived.
        EV << "Message " << ttmsg << " arrived after " << ttmsg->getHopCount()
        << " hops.\n";
        bubble("ARRIVED, starting new one!");
        numReceived++;
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);

        delete ttmsg;

        // Generate another one.
        EV << "Generating another message: ";
        SimpleMessage *newmsg = generateMessage();
        EV << newmsg << endl;
        numSent++;
        forwardMessage(newmsg);
    }
    else {
        // We need to forward the message.
        forwardMessage(ttmsg);
    }
}

SimpleMessage *Computer::generateMessage()
{
        // Produce source and destination addresses.
        int src = getIndex(); // our module index
        int n = getVectorSize(); // module vector size
        int dest = intuniform(0, n-2);
        if (dest >= src)
            dest++;
        char msgname[20];
        sprintf(msgname, "from-%d-to-%d", src, dest);

        // Create message object and set source and destination field.
        SimpleMessage *msg = new SimpleMessage(msgname);
        msg->setSource(src);
        msg->setDestination(dest);
        return msg;
}

void Computer::forwardMessage(SimpleMessage *msg)
{
    cMessage *ttmsg = check_and_cast<cMessage *>(msg);
        // Increment hop count.
        msg->setHopCount(msg->getHopCount()+1);

        int n = gateSize("gate");
        int k = intuniform(0, n-1);
        int sender;
        if(ttmsg->getArrivalGate() != NULL){
            if(gateSize("gate$o") != 1)
            sender = ttmsg->getArrivalGate()->getIndex();
            bool find = true;
                    while (find){
                        if(k != sender){
                            find = false;
                        }
                        else
                            k = intuniform(0, n-1);
                    }
        }
        // Same routing as before: random gate but send to the sender.

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
        send(msg, "gate$o", k);

}

void Computer::finish(){

    EV << "===============  === Report ===  ===============\n";

    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:              " << numSent << endl;
    EV << "Received:          " << numReceived << endl;
    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;
    EV << "===============  === ====== ===  ===============\n";
    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);

    hopCountStats.recordAs("hop count");
}

