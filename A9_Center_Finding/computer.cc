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

//1. the activation stage, started by the initiators, in which all nodes are activated;
//2. the saturation stage, started by the leaf nodes, in which a unique couple of
//neighboring nodes is selected; and
//3. the resolution stage, started by the selected pair.

#include "computer.h"
#include "SimpleMessage_m.h"
#include<string.h>
#include<stdio.h>
#include<map>

Define_Module(Computer);



void Computer::initialize()
{

    // Tout les noeuds sont dans l'etat actif
    for(int i = 0; i < gateSize("gate$o"); i++)
        {
            Vos.insert(i); // Ajouter tout les Voinsins
        }
    // initialize
    Counter =  gateSize("gate$o") -1;
    if(gateSize("gate$o") == 1)
        {
            // prepare message
            std::set<int>::iterator T = Vos.begin();
            parent = *T;
            EV<< *T<<"\n";
            SimpleMessage *nmsg = new SimpleMessage("M");
            nmsg->setSource(getIndex());
            nmsg->setKind(1);  // kind = 1 <-> le msg est  M
            nmsg->setMaxdist(0); // les noeud externe envoi 0
            send(nmsg,"gate$o",0); // envoi a leur parent
            processing = true;  // become proccecing
        }
    else{
        active = true; // become active
    }
}

void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);
    if(active){
            int sender = msg->getArrivalGate()->getIndex();
            Vos.erase(sender);
            if(Nmsg->getMaxdist() + 1 > maxdist)
                maxdist = Nmsg->getMaxdist() + 1;
            distance[sender] = Nmsg->getMaxdist() + 1;
            if(Vos.size() == 1){ // il reste le parent
                // process_Message
                std::set<int>::iterator T = Vos.begin();
                parent = *T;
                SimpleMessage *nmsg = new SimpleMessage("Saturation");
                nmsg->setSource(getIndex());
                nmsg->setKind(1);  // kind = 2 <-> le msg est Saturation
                nmsg->setMaxdist(maxdist);
                send(nmsg,"gate$o",parent);
                processing = true;
                active = false;
            }
            EV <<" size is "<<Vos.size()<<"\n";
    }
    else{
        if(processing){
            if(Nmsg->getKind() == 1) // le msg est Saturation
            {
                saturated = true;  // Become Sturated
                bubble("I am Saturated");
                cDisplayString& b = this->getDisplayString();
                b.parse("i=block/broadcast,red");
                parent = msg->getArrivalGate()->getIndex();
                int maxParent = Nmsg->getMaxdist() +1 ;
                if(maxParent  >= maxdist)
                {
                    Ecn = maxParent;  // le plus petit Ecn  que le noeud voie
                    MinEcn  = Ecn;
                }
                else
                {
                    Ecn = maxdist;
                    MinEcn  = Ecn;
                }
                for ( int i = 0; i < gateSize("gate$o"); i++)
                {
                    if ( i != msg->getArrivalGate()->getIndex())
                    {
                        SimpleMessage *nmsg = new SimpleMessage("Resolution");
                        nmsg->setKind(2);  // le msg est Resolution
                        nmsg->setMaxdist(Nmsg->getMaxdist() + 1);
                        send(nmsg,"gate$o",i);
                    }
                }
                done= true; // les 2 noeud saturé terminent leurs travaille devient saturés
                saturated = true;
                processing = false;
                // Resolve probleme
            }
            else{
                if(Nmsg->getKind() == 2){ // le msg est Resolution
                    int maxParent = Nmsg->getMaxdist() + 1;
                      if(maxParent >= maxdist)
                      {
                          Ecn = maxParent;
                          MinEcn  = Ecn;
                      }
                      else
                      {
                          Ecn = maxdist;
                          MinEcn  = Ecn;
                      }
                      for ( int i = 0; i < gateSize("gate$o"); i++)
                         {
                             if ( i != msg->getArrivalGate()->getIndex())
                             {
                                 SimpleMessage *nmsg = new SimpleMessage("Resolution");
                                 nmsg->setKind(2);  // le msg est Resolution
                                 nmsg->setMaxdist(Nmsg->getMaxdist() + 1);
                                 send(nmsg,"gate$o",i);
                             }
                         }
                      done = true;
                      processing = false;
                      if(gateSize("gate$o") == 1)  // les noeuds externes
                      {
                          SimpleMessage *nmsg = new SimpleMessage("MinimumEcn");
                          nmsg->setKind(3);  // le msg est Minimum Ecn
                          nmsg->setMaxdist(Ecn); // il envoie leur Ecn
                          send(nmsg,"gate$o",0);
                      }

                }
            }
        }
        else
        {
            if(done){
            if(Nmsg->getKind() == 3){
                Counter--;
             if(MinEcn >= Nmsg->getMaxdist())
                MinEcn = Nmsg->getMaxdist();

            if(Counter == 0) // tout les Voisin sauf le parent sont envoyés un message de Minimum Ecn
            {
                SimpleMessage *nmsg = new SimpleMessage("MinimumEcn");
                nmsg->setKind(3);  // le msg est Minimum Ecn
                nmsg->setMaxdist(MinEcn); // il envoie leur Ecn
                send(nmsg,"gate$o",parent);
            }
            if(Counter == -1)  // le noeud sature envoi a leur pere un msg Minimum Ecn
            {
                 if(saturated){ // assure que est noeud saturé
                     Center = MinEcn;
                     if(Ecn == Center)
                         amCenter = true;
                     for ( int i = 0; i < gateSize("gate$o"); i++)
                      {
                          if ( i != parent)
                          {
                              SimpleMessage *nmsg = new SimpleMessage("Broadcast");
                              nmsg->setKind(4);  // le msg est Broadcast
                              nmsg->setMaxdist(Center);  // il envoie le Centre (Ecn de centre)
                              send(nmsg,"gate$o",i);
                          }
                      }
                 }
            }
        }
            else{
                if(Nmsg->getKind() == 4)
                {
                    Center = Nmsg->getMaxdist();
                    if(Center == Ecn)
                        amCenter = true;
                    for ( int i = 0; i < gateSize("gate$o"); i++)
                      {
                          if ( i != parent)
                          {
                              SimpleMessage *nmsg = new SimpleMessage("Broadcast");
                              nmsg->setKind(4);  // le msg est Broadcast
                              nmsg->setMaxdist(Nmsg->getMaxdist());  // il envoie le Centre (Ecn de centre)
                              send(nmsg,"gate$o",i);
                          }
                      }
                }
            }
    }
        }
    }
}
void Computer::finish(){

    EV << "===============  === Report ===  ===============\n";

    // This function is called by OMNeT++ at the end of the simulation.
    EV << "    Eccentricitie:       " << Ecn << endl;
    EV << "    Le Centre a une Eccentricitie      " <<Center<<endl;
    EV << "    je suis le centre                  " << amCenter<<endl;


    EV << "===============  === ====== ===  ===============\n";

}

