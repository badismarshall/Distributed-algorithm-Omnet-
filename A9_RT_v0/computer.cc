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
    // Chaque noeud Envoye une ND-Request
    counter = 0;
    for (int i = 0; i < gateSize("gate$o"); i++){
           SimpleMessage *msg = new SimpleMessage("ND-Request");
           msg->setSource(getIndex());
           msg->setNeighbor(-1);
           msg->setDestination(-1);
           send(msg,"gate$o",i);
    }
}

void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);
    if(Nmsg->getNeighbor() == -1){   // le msg est un ND-Request
        int sender = gate("gate$o",msg->getArrivalGate()->getIndex())->getPathEndGate()->getOwnerModule()->getIndex();
        SimpleMessage *Tmsg = new SimpleMessage("ND-Ack");
        Tmsg->setSource(sender);
        Tmsg->setNeighbor(getIndex());
        Tmsg->setDestination(-1);
        send(Tmsg,"gate$o",msg->getArrivalGate()->getIndex());
    }
    else{
        if(Nmsg->getDestination() == -1)
        {
         int sender = Nmsg->getNeighbor();
         Vos.insert(sender);
         counter++;
         if (counter == gateSize("gate$o"))
             {
                 int n = getVectorSize();
                 int iniator = intuniform(0, n-2);
                 int dest = intuniform(0, n-2);
                 if(getIndex() == iniator) // genere un message pour l'envoyée dans le réseau
                 {
                        while(dest == getIndex())
                            dest = intuniform(0, n-1);
                        if(Vos.find(dest) != Vos.end())
                        { // le destinateur est un Voisin
                            std::set<int>::iterator neigb = Vos.find(dest);
                            for(int i = 0; i< gateSize("gate$o"); i++){
                                if(*neigb == gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()){

                                    char msgname[20];
                                    sprintf(msgname, "from-%d-to-%d", getIndex(), dest);
                                    numSent++;
                                    SimpleMessage *Tmsg = new SimpleMessage(msgname);
                                    Tmsg->setSource(getIndex());
                                    Tmsg->setDestination(dest);
                                    Tmsg->setNeighbor(0);
                                    Tmsg->setHopCount(0);
                                    send(Tmsg,"gate$o",i);
                                    break;
                                }

                            }

                        }
                        else
                        { // le distinateur n'existe pas dans la table de routage
                            int T = intuniform(0, gateSize("gate$o")-1);
                            char msgname[20];
                            numSent++;
                            sprintf(msgname, "from-%d-to-%d", getIndex(), dest);
                            SimpleMessage *Tmsg = new SimpleMessage(msgname);
                            Tmsg->setSource(getIndex());
                            Tmsg->setDestination(dest);
                            Tmsg->setNeighbor(0);
                            Tmsg->setHopCount(0);
                            send(Tmsg,"gate$o",T);
                        }
             }

         }

        }
        else{
            int dest = Nmsg->getDestination();
            if(getIndex() == dest)
            {
                int hopcount = Nmsg->getHopCount();
                // Message arrived.
                EV << "Message " << Nmsg << " arrived after " << Nmsg->getHopCount()
                << " hops.\n";
                bubble("ARRIVED, starting new one!");
                numReceived++;
                hopCountVector.record(hopcount);
                hopCountStats.collect(hopcount);
                // le paquet arrivée au destinateur
                bubble("Done paquet arrived");

                // Generate another one.
                EV << "Generating another message: ";
                numSent++;
                forwardMessage();

            }
            else
            {
                // cherche si le distinateur est un  voisin
                if(Vos.find(dest) != Vos.end())
                       { // le destinateur est un Voisin
                           std::set<int>::iterator neigb = Vos.find(dest);
                           for(int i = 0; i< gateSize("gate$o"); i++){
                               if(*neigb == gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()){

                                   char msgname[20];
                                   sprintf(msgname, "from-%d-to-%d", Nmsg->getSource(), dest);

                                   SimpleMessage *Tmsg = new SimpleMessage(msgname);
                                   Tmsg->setSource(Nmsg->getSource());
                                   Tmsg->setDestination(dest);
                                   Tmsg->setNeighbor(0);
                                   Tmsg->setHopCount(Nmsg->getHopCount()+1);
                                   send(Tmsg,"gate$o",i);
                                   break;
                               }

                           }

                       }
            else
            {
                    // le destinateur n'est pas voisin
                    int T = intuniform(0, gateSize("gate$o")-1);
                    if(gateSize("gate$o") != 1)
                    {
                        while ( T == msg->getArrivalGate()->getIndex())
                        {
                            T = intuniform(0, gateSize("gate$o")-1);
                        }
                    }
                    char msgname[20];
                    sprintf(msgname, "from-%d-to-%d", Nmsg->getSource(), dest);
                    SimpleMessage *Tmsg = new SimpleMessage(msgname);
                    Tmsg->setSource(Nmsg->getSource());
                    Tmsg->setDestination(dest);
                    Tmsg->setNeighbor(0);
                    Tmsg->setHopCount(Nmsg->getHopCount()+1);
                    send(Tmsg,"gate$o",T);
            }
            }
        }
    }

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

void Computer::forwardMessage()
{
                            int n = getVectorSize();
                            int dest = intuniform(0, n-1);
                            while(dest == getIndex())
                                dest = intuniform(0, n-1);
                            if(Vos.find(dest) != Vos.end())
                            { // le destinateur est un Voisin
                                std::set<int>::iterator neigb = Vos.find(dest);
                                for(int i = 0; i< gateSize("gate$o"); i++){
                                    if(*neigb == gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()){

                                        char msgname[20];
                                        sprintf(msgname, "from-%d-to-%d", getIndex(), dest);
                                        SimpleMessage *Tmsg = new SimpleMessage(msgname);
                                        Tmsg->setSource(getIndex());
                                        Tmsg->setDestination(dest);
                                        Tmsg->setNeighbor(0);
                                        Tmsg->setHopCount(0);
                                        send(Tmsg,"gate$o",i);
                                        break;
                                    }

                                }

                            }
                            else
                            { // le distinateur n'existe pas dans la table de routage
                                int T = intuniform(0, gateSize("gate$o")-1);
                                char msgname[20];
                                sprintf(msgname, "from-%d-to-%d", getIndex(), dest);
                                SimpleMessage *Tmsg = new SimpleMessage(msgname);
                                Tmsg->setSource(getIndex());
                                Tmsg->setDestination(dest);
                                Tmsg->setNeighbor(0);
                                Tmsg->setHopCount(0);
                                send(Tmsg,"gate$o",T);
                            }


}
