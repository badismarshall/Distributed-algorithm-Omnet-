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
    // l'iniateur d'index 0
    if(getIndex() == 0)
    {
        for(int i = 0; i < gateSize("gate$o"); i++)
        {
            //int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
            Vos.insert(i);
        }
        // Chosit un Voisin pour l'envoyée le Token
        int To = intuniform(0,gateSize("gate$o"));
        Vos.erase(To);
        iniator = true;   // l'iniaruer est le pere de lui
        done = true;
        SimpleMessage *TOC = new SimpleMessage("Token");
        TOC->setSource(getIndex());
        send(TOC,"gate$o",To);
        EV <<" le iniateur send message To " << gate("gate$o",To)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
    }
}

void Computer::handleMessage(cMessage *msg)
{
   if(!done)
   {
       iniator = false;
       for(int i = 0; i < gateSize("gate$o"); i++)
       {
        //int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
        Vos.insert(i);
       }
        done = true;
    }

    SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);
    if(Pere == -1)
    {
        Pere = msg->getArrivalGate()->getIndex();
        Vos.erase(Pere);
        //Vos.erase(Pere);
    }
    if(Vos.empty())
    {
        if(!iniator)
        {
            SimpleMessage *TOC = new SimpleMessage("Token");
            TOC->setSource(getIndex());
            send(TOC,"gate$o",Pere);
            EV <<getIndex()<<" send token TO"<< Pere <<"\n";
        }
    }
    if(!Vos.empty())
    {
         std::set<int>::iterator To = Vos.begin();
         Vos.erase(To);
         SimpleMessage *TOC = new SimpleMessage("Token");
         TOC->setSource(getIndex());
         send(TOC,"gate$o",*To);
         EV <<" send token TO "<< *To <<"\n";
    }
}



