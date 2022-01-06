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


void Computer::first_visit(int sender){
    iniator = false;
    entry = sender;
    Unvisited.erase(sender);
    if(!Unvisited.empty()){
        std::set<int>::iterator T = Unvisited.begin();
        this->next = *T;
        Unvisited.erase(*T);
        SimpleMessage *nmsg = new SimpleMessage("Token");
        nmsg->setKind(1);  // envoyer le Token
        send(nmsg,"gate$o",*T);
        idle = false;
        available = false;
        visited =true;
        for(int i = 0; i <gateSize("gate$o");i++){
               if(i != next && i != entry){
               SimpleMessage *nmsg = new SimpleMessage("visited");
               nmsg->setKind(4);  // envoyer le visited
               send(nmsg,"gate$o",i);
           }
        }
    }
    else{
            SimpleMessage *nmsg = new SimpleMessage("Return");
            nmsg->setKind(2);  // envoyer le Return
            send(nmsg,"gate$o",entry);
            cDisplayString& connDispStr = gate("gate$o",entry)->getDisplayString();
            connDispStr.parse("ls=blue");
            done = true;
            for(int i = 0; i <gateSize("gate$o");i++){
               if(i != entry){
                   SimpleMessage *nmsg = new SimpleMessage("visited");
                   nmsg->setKind(4);  // envoyer le visited
                   send(nmsg,"gate$o",i);
               }
            }
            done = true;
            idle = false;
            available = false;
    }
}
void Computer::visit(){
    if(!Unvisited.empty()){
        std::set<int>::iterator T = Unvisited.begin();
        Unvisited.erase(*T);
        this->next = *T;
        SimpleMessage *nmsg = new SimpleMessage("Token");
        nmsg->setKind(1);  // envoyer le Token
        send(nmsg,"gate$o",*T);
    }
    else{
        if(!iniator){
            SimpleMessage *nmsg = new SimpleMessage("Return");
            nmsg->setKind(2);  // envoyer le Return
            send(nmsg,"gate$o",entry);
            cDisplayString& connDispStr = gate("gate$o",entry)->getDisplayString();
            connDispStr.parse("ls=blue");
            done = true;
            idle = false;
            available = false;
        }
    }
}

void Computer::initialize()
{
        if(getIndex() == 0)
        {
            iniator = true;
            EV<< " i am the iniator "<< getIndex()<<" \n";
            for(int i = 0; i < gateSize("gate$o"); i++)   // Unvisited = N(x)
                 {
                    Unvisited.insert(i);
                 }

            visited = true;
            idle = false;
            std::set<int>::iterator T = Unvisited.begin();
            Unvisited.erase(*T);
            this->next = *T;
            EV << "   the next is " << next<<"\n";
            SimpleMessage *nmsg = new SimpleMessage("Token");
            nmsg->setKind(1);  // envoyer le Token
            send(nmsg,"gate$o",*T);
            for(int i = 0; i <gateSize("gate$o");i++)
            {
                if(i != next){
                SimpleMessage *nmsg = new SimpleMessage("visited");
                nmsg->setKind(4);  // envoyer le visited
                send(nmsg,"gate$o",i);
                }
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
    if(idle == true){
        if(nNmsg->getKind() == 1) // le msg est le Token
        {
            int sender = msg->getArrivalGate()->getIndex();
            parent = sender;
            cDisplayString& connDispStr = gate("gate$o",parent)->getDisplayString();
            connDispStr.parse("ls=blue");
            first_visit(sender);
        }
        else{
            if(nNmsg->getKind() == 4){  // le msg est visited
                int sender =   msg->getArrivalGate()->getIndex();
                Unvisited.erase(sender);
                available = true;
                idle = false;
                visited = false;
            }
        }
    }
    else{
        if(available == true){
            if(nNmsg->getKind() == 1){
                first_visit(msg->getArrivalGate()->getIndex());
            }
            else{
                if(nNmsg->getKind() == 4){
                    int sender = msg->getArrivalGate()->getIndex();
                    Unvisited.erase(sender);
                }
            }
        }
        else{
            if(visited == true){
                if(nNmsg->getKind() == 4 || nNmsg->getKind() == 1){
                    int sender = msg->getArrivalGate()->getIndex();
                    Unvisited.erase(sender);
                    std::set<int>::iterator T = Unvisited.begin();
                    next = int(*T);
                    if(next == sender){
                        visit();
                    }
                }
                else{
                    if(nNmsg->getKind() == 2){
                        cDisplayString& connDispStr = gate("gate$o",msg->getArrivalGate()->getIndex())->getDisplayString();
                        connDispStr.parse("ls=blue");
                        visit();
                    }
                }
            }
        }
    }
}














