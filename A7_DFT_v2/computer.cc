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
        if(getIndex() == 0)
        {
            EV<< " i am the iniator "<< getIndex()<<" \n";
            for(int i = 0; i < gateSize("gate$o"); i++)
                 {
                    Unvisited.insert(i);
                 }
            for(std::set<int>::iterator i = Unvisited.begin() ; i != Unvisited.end(); ++i)
                  {
                            EV<<" "<<*i;
                  }
                    EV<< "\n";
            iniator = true;
            visited = true;
            std::set<int>::iterator T = Unvisited.begin();
            Unvisited.erase(*T);
            SimpleMessage *nmsg = new SimpleMessage("Token");
            nmsg->setKind(1);  // envoyer le Token
            send(nmsg,"gate$o",*T);
            for(int i = 0; i <gateSize("gate$o");i++)
            {
                SimpleMessage *nmsg = new SimpleMessage("visited");
                nmsg->setKind(4);  // envoyer le visited
                send(nmsg,"gate$o",i);
            }
        }
        else
        {
            for(int i = 0; i < gateSize("gate$o"); i++)
                {
                    Unvisited.insert(i);
                }
                        iniator = false;
                        idle =true;

        }



    }


void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *nNmsg = check_and_cast<SimpleMessage *>(msg);
    if(nNmsg->getKind() == 2) // le msg est un return de Token
    {
        if(Unvisited.empty())
                   {
                       if(!iniator){
                           SimpleMessage *nmsg = new SimpleMessage("Return");
                           nmsg->setKind(2);  // msg return
                           send(nmsg,"gate$o",entry);
                       }
                   }
        else{
                       std::set<int>::iterator dest = Unvisited.begin();
                       SimpleMessage *nmsg = new SimpleMessage("Token");
                       nmsg->setKind(1);  // msg token
                       send(nmsg,"gate$o",*dest);
                   }
    }
   if((idle && nNmsg->getKind() == 4) || (visited && nNmsg->getKind() == 4))
   {
        int sender =   msg->getArrivalGate()->getIndex();
        Unvisited.erase(sender);
        idle = false;
        visited = true;
   }
   else
   {
       if((idle && nNmsg->getKind() == 1) || (visited && nNmsg->getKind() == 1))
       {
           if (entry == -1)
               entry = msg->getArrivalGate()->getIndex();
           int sender = msg->getArrivalGate()->getIndex();
           Unvisited.erase(sender);
           idle= false;
           visited = true;
           for(int i =0; i < gateSize("gate$o"); i++)
                          {
                             SimpleMessage *nmsg = new SimpleMessage("Visited");
                             nmsg->setKind(4);  // msg visited
                             send(nmsg,"gate$o",i);
                          }
           if(!Unvisited.empty())
           {
               std::set<int>::iterator T =Unvisited.begin();
               SimpleMessage *nmsg = new SimpleMessage("Token");
               nmsg->setKind(1);  // msg Token
               send(nmsg,"gate$o",*T);
           }
           else
           {
               SimpleMessage *nmsg = new SimpleMessage("Return");
               nmsg->setKind(2);  // msg Token
               send(nmsg,"gate$o",entry);
           }

       }

   }
}














