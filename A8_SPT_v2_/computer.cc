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


                                // kind = 1 Question // kind = 2 Yes //  kind = 3 No
void Computer::initialize()
{
        if(getIndex() == 0)
        {
            root = true;
            EV<< " i am the iniator "<< getIndex()<<" \n";
            for(int i = 0; i < gateSize("gate$o"); i++)
                {
                    int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                    Vos.insert(Voisin); // Ajouter tout les Voinsins de l'initiateur
                }
                qs = gateSize("gate$o");
            for(int i = 0; i < gateSize("gate$o"); i++)
                 {
                    SimpleMessage *nmsg = new SimpleMessage("Question");
                    nmsg->setKind(1);  // Envoyé une Question
                    nmsg->setZ(Vos);
                    send(nmsg,"gate$o",i);
                 }
            counter = 0;
            active = true;
        }
        else
        {
            for(int i = 0; i < gateSize("gate$o"); i++)
               {
                   int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                   Vos.insert(Voisin);
               }
                        idle = true;
        }



    }


void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *nNmsg = check_and_cast<SimpleMessage *>(msg);
    if(idle){
        if(nNmsg->getKind() == 1) // le msg est la Question
        {
            root = false;
            parent = msg->getArrivalGate()->getIndex();
            St.insert(parent);
            //SimpleMessage *nmsg = new SimpleMessage("Yes");
            //nmsg->setKind(2);  // répondre avec Yes
            //send(nmsg,"gate$o",parent);
            //cDisplayString& connDispStr = gate("gate$o",msg->getArrivalGate()->getIndex())->getDisplayString();
            //connDispStr.parse("ls=red");
            counter = 1;
            if(counter == gateSize("gate$o")){
                idle =false;
                done = true;
                active = false;
                if(!root){
                     SimpleMessage *M = new SimpleMessage("Yes");
                     M->setKind(2); // Envoyée le Yes
                     M->setZ(nNmsg->getZ());
                     send(M,"gate$o",parent);
                     cDisplayString& connDispStr = gate("gate$o",parent)->getDisplayString();
                     connDispStr.parse("ls=red");
                }
                bubble("Done !");
            }
            else
            {
                set<int> Zx = nNmsg ->getZ(); // Recever Z
                // Y les Vosins que chaque noeud doit l'envoyeé le msg
                set<int> Y;   // = Vos - Zx;
                std::set_difference(Vos.begin(),Vos.end(),
                                   Zx.begin(),Zx.end(),
                                   std::inserter(Y, Y.begin()));
                int sender = gate("gate$o",msg->getArrivalGate()->getIndex())->getPathEndGate()->getOwnerModule()->getIndex();
                Y.erase(sender);
                qs = Y.size();
                EV<<" I AM "<<getIndex()<<" My qs is  "<<qs<<"\n";
                if(!Y.empty()){
                    set<int> NZx;
                    std::set_union(Y.begin(),Y.end(),
                                        Zx.begin(),Zx.end(),
                                        std::inserter(NZx,NZx.begin()));
                    NZx.insert(sender);
                    int Nx;
                    for(int i = 0; i < gateSize("gate$o"); i++)
                      {
                          Nx = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();

                          if(Y.find(Nx) != Y.end())
                          {
                              SimpleMessage *M = new SimpleMessage("Question");
                              M->setKind(1); // Envoyée le Question
                              M->setZ(NZx);
                              send(M,"gate$o",i);
                          }
                      }
                    active = true;
                    idle =false;
                }
                else
                {
                     SimpleMessage *M = new SimpleMessage("Yes");
                     M->setKind(2); // Envoyée le Yes
                     M->setZ(Zx);
                     send(M,"gate$o",parent);
                     cDisplayString& connDispStr = gate("gate$o",msg->getArrivalGate()->getIndex())->getDisplayString();
                     connDispStr.parse("ls=red");
                     done = true ;
                     idle = false;
                }

            }
        }
    }
    else
    {
                if(active){
                    if(nNmsg->getKind() == 1){  // Recever Question
                        qs--;
                        counter ++;
                        if(qs == 0){
                            done = true;
                            active = false;
                            idle = false;
                            if(!root){
                                SimpleMessage *M = new SimpleMessage("Yes");
                                M->setKind(2); // Envoyée le Yes
                                M->setZ(nNmsg->getZ());
                                send(M,"gate$o",parent);
                                cDisplayString& connDispStr = gate("gate$o",parent)->getDisplayString();
                                connDispStr.parse("ls=red");
                                bubble("Done !");
                            }
                        }
                        /*set<int> Zx = nNmsg ->getZ(); // Recever Z
                         int sender = msg->getArrivalGate()->getIndex();
                         SimpleMessage *M = new SimpleMessage("No");
                         M->setKind(3); // Envoyée le No
                         send(M,"gate$o",sender);*/
                    }
                    else
                    {
                        if(nNmsg->getKind() == 2){ // Recever Yes
                             int sender = msg->getArrivalGate()->getIndex();
                             St.insert(sender);
                             qs--;
                             counter ++;
                             cDisplayString& connDispStr = gate("gate$o",msg->getArrivalGate()->getIndex())->getDisplayString();
                             connDispStr.parse("ls=red");
                             if(qs == 0)
                             {
                                done = true;
                                active =false;
                                idle = false;
                                if(!root){
                                     SimpleMessage *M = new SimpleMessage("Yes");
                                     M->setKind(2); // Envoyée le Yes
                                     M->setZ(nNmsg->getZ());
                                     send(M,"gate$o",parent);
                                     cDisplayString& connDispStr = gate("gate$o",parent)->getDisplayString();
                                     connDispStr.parse("ls=red");
                                     bubble("Done !");
                              }
                             }
                        }
                    }
                }
    }
}
