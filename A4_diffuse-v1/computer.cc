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
    if(this->done==0)
    {
        if (getIndex() == 0) {
        // Boot the bcast process
            for (int i=0; i<gateSize("gate$o"); i++)
                {
                    SimpleMessage *msg = copymessage();
                    msg->setSource(getIndex());
                    sendcopy(msg,i);
                }
        }
    }
    else
    {
        this->done = 1;
        return;
    }
}

void Computer::handleMessage(cMessage *msg)
{

    if(this->done==0)
    {
        if(getIndex() == 0)
        {
        }
        else
            {
                this->done = 1;
                SimpleMessage *smsg = check_and_cast<SimpleMessage *>(msg);
                int  j = msg->getArrivalGate()->getIndex();
                for (int i=0; i<gateSize("gate$o"); i++)
                    {
                        if(j!=i)
                        {
                            SimpleMessage *ssmsg = copymessage();
                            ssmsg->setSource(this->getIndex());
                            sendcopy(ssmsg,i);
                        }
                    }
            }
    }
    else
    {
        return ;
    }
}

SimpleMessage* Computer:: copymessage(){
    SimpleMessage *my_msg = new SimpleMessage("Hi");
    return my_msg;
}

void Computer::sendcopy(SimpleMessage *msg,int i){
    cMessage *smsg = (cMessage *)msg->dup();
    send(smsg,"gate$o",i);
}
