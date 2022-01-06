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
Define_Module(Computer);

void Computer::initialize()
{
    if (getIndex() == 0) {
        // Boot the bcast process
        for (int i=0; i<gateSize("gate$o"); i++)
        {
            cMessage *msg = copymessage();
            sendcopy(msg,i);
        }
    }
    return ;
}

void Computer::handleMessage(cMessage *msg)
{
    if(getIndex() == 0)
    {
    }
    else
    {
       for (int i=0; i<gateSize("gate$o"); i++)
            {
                sendcopy(msg,i);
            }
    }
}

cMessage* Computer:: copymessage(){
    cMessage *my_msg = new cMessage("Hi");
    return my_msg;
}

void Computer::sendcopy(cMessage *msg,int i){
    cMessage *copy = (cMessage *)msg->dup() ;
    send(copy,"gate$o",i);
}
