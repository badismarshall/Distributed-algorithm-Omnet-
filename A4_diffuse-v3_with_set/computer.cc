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

int MessagesN = 0;
int NombresEdges = 0;
            // Initiateur Unique
void Computer::initialize()
{
    hopCountStats.setName("hopCountStats");
    hopCountStats.setRange(0, 10);
    hopCountVector.setName("MessagesN");
    if(getIndex() == 0)
    {
        for(int i = 0; i < gateSize("gate$o"); i++)
        {
            int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
            Vos.insert(Voisin); // Ajouter tout les Voinsins de l'initiateur
        }
        for (int i = 0; i < gateSize("gate$o"); i++)
        {
            SimpleMessage *msg = new SimpleMessage("M");
            Vos.insert(getIndex());
            msg->setZ(Vos);
            send(msg,"gate$o",i);
            EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
        }
        done = true;
    }
}

void Computer::handleMessage(cMessage *msg)
{
    MessagesN++;
    if(getIndex() != 0 && !done) // tout les Noeud Sauf l'iniateur
    {
        SimpleMessage *Nmsg = check_and_cast<SimpleMessage *>(msg);
        // La liste récu de sender
        set<int> Zx = Nmsg ->getZ();
        for(int i = 0; i < gateSize("gate$o"); i++)
               {
                   int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                   Vos.insert(Voisin); // Ajouter tout les Voinsins de l'initiateur
               }
        // Y les Vosins que chaque noeud doit l'envoyeé le msg
        set<int> Y;   // = Vos - Zx;
        std::set_difference(Vos.begin(),Vos.end(),
                            Zx.begin(),Zx.end(),
                            std::inserter(Y, Y.begin()));
        done = true;
        if(!Y.empty())
        {
            // Zx = Y.Union(Zx) + getIndex(); // la liste que chaque noeud doit le transmetée
            set<int> NZx;
            std::set_union(Y.begin(),Y.end(),
                                Zx.begin(),Zx.end(),
                                std::inserter(NZx,NZx.begin()));
            int Nx;
            for(int i = 0; i < gateSize("gate$o"); i++)
            {
                Nx = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();

                if(Y.find(Nx) != Y.end())
                {
                    SimpleMessage *M = new SimpleMessage("M");
                    M->setSource(getIndex());
                    M->setZ(NZx);
                    send(M,"gate$o",i);
                    EV<<getIndex()<<" Send to "<< gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex()<<"\n";
                }
            }

        }

    }

}

void Computer::finish(){
    if(getIndex() == 0 ){
        hopCountVector.record(MessagesN);
        hopCountVector.record(NombresEdges/2);
        hopCountStats.collect(MessagesN);
        EV<<"================================"<<endl;
        EV<< NombresEdges;
    }
}
