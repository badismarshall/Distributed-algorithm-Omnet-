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
#include <bits\stdc++.h>
#include<set>

using namespace omnetpp;

using namespace std;
Define_Module(Computer);

void Computer::initialize()
{
    int awake = intuniform(0,1);
    if(awake == 1)
    {
        for (int i = 0; i < gateSize("gate$o"); i++)
        {
            SimpleMessage *msg = new SimpleMessage("M");
            msg->setDestiontion(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex());
            msg->setSource(getIndex());
            send(msg,"gate$o",i);
            EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
        }
        done = true;
    }
}

void Computer::handleMessage(cMessage *msg)
{
    if(getIndex() != 0 && getIndex() != 3 && !done) // tout les Noeud Sauf l'iniateur
    {
        SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);

        for(int i = 0; i < gateSize("gate$o"); i++)
               {
                  if(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex() != Nmsg->getSource())
                  {
                      SimpleMessage *msg = new SimpleMessage("M");
                      msg->setDestiontion(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex());
                      msg->setSource(getIndex());
                      send(msg,"gate$o",i);
                      EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
                  }
               }
        done = true;

}
}
