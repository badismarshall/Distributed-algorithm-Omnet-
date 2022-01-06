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
void Computer::visit()
{
    if(!Unvisited.empty())
    {
        std::set<int>::iterator next = Unvisited.begin();
        Unvisited.erase(*next);
        for(int i = 0; i < gateSize("gate$o"); i++)
        {
            if(i == *next)
            {
                SimpleMessage *nmsg = new SimpleMessage("Token");
                nmsg->setKind(1);
                nmsg->setSource(getIndex());
                idle = false;
                visited = true;
                send(nmsg,"gate$o",i);
                EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
              }
          }
    }
    else
    {
        if(!iniator)
        {
            for(int i = 0; i < gateSize("gate$o"); i++)
                   {
                       if(i == entry)
                       {
                           SimpleMessage *nmsg = new SimpleMessage("return");
                           nmsg->setKind(2);
                           nmsg->setSource(getIndex());
                           idle = false;
                           visited =false;
                            done = true;
                           send(nmsg,"gate$o",i);
                           EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
                         }
                     }

        }
    }
}
void Computer::initialize()
{
        if(getIndex() == 0)
        {
            EV<< " i am the iniator "<< getIndex()<<" \n";
            for(int i = 0; i < gateSize("gate$o"); i++)
                 {
                    //int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                    //Unvisited.insert(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex());
                     Unvisited.insert(i);
                 }
            for(std::set<int>::iterator i = Unvisited.begin() ; i != Unvisited.end(); ++i)
                  {
                            EV<<" "<<*i;
                  }
                    EV<< "\n";
            iniator = true;
            visit();
        }
        else
        {
            for(int i = 0; i < gateSize("gate$o"); i++)
                {
                      //int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                      //Unvisited.insert(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex());
                    Unvisited.insert(i);
                }
                        iniator = false;
                        idle =true;

        }



    }

void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *nNmsg = check_and_cast<SimpleMessage *>(msg);
   if(idle && nNmsg->getKind() == 1)
   {
        entry =  msg->getArrivalGate()->getIndex();
        Unvisited.erase(entry);
        visit();
   }
   else
   {
   if(visited)
   {
       SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);
       int kind = Nmsg->getKind();
       if(kind == 1) // le msg est un token
       {
           int sender = Nmsg->getArrivalGate()->getIndex();
           Unvisited.erase(sender);
           SimpleMessage *nmsg = new SimpleMessage("backedge");
                                   nmsg->setKind(3);
                                   nmsg->setSource(getIndex());
                                   int i = msg->getArrivalGate()->getIndex();
                                   send(nmsg,"gate$o",i);
           /*for(int i = 0; i < gateSize("gate$o"); i++)
                {
                    if(gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex() == sender)
                    {
                        SimpleMessage *nmsg = new SimpleMessage("backedge");
                        nmsg->setKind(3);
                        nmsg->setSource(getIndex());
                        send(nmsg,"gate$o",i);
                    }
                }*/
       }
       else
       {
           if(kind == 2 || kind == 3)
           {
               visit();
           }
       }
   }
   }
}



