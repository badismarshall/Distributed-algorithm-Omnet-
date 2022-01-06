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
#include<iterator>
#include<algorithm>

using namespace omnetpp;

using namespace std;
Define_Module(Computer);



void Computer::initialize()
{

            for(int i = 0; i < gateSize("gate$o"); i++)
                {
                    int Voisin = gate("gate$o",i)->getPathEndGate()->getOwnerModule()->getIndex();
                    Unknown.insert(Voisin); // Ajouter tout les Voinsins comme Unknown
                }
             // Intialiser une coleur
            Macouleur = intuniform(0,1);  // 0 noir ; 1 blanc
            if(Macouleur == 0)
                Black_nodes.insert(getIndex());
            else
                White_nodes.insert(getIndex());
            if(Macouleur == 0) {
                cDisplayString& dd = this->getDisplayString();
                dd.parse("i=block/routing,black");
                }
            // deffuse l'information
            for(int i =0; i < gateSize("gate$o"); i++){
                SimpleMessage *nmsg = new SimpleMessage("Message");
                nmsg->setBlack(Black_nodes);
                nmsg->setWhite(White_nodes);
                nmsg->setUnknown(Unknown);

                send(nmsg,"gate$o",i);
            }
            idle = true;
    }


void Computer::handleMessage(cMessage *msg)
{
    SimpleMessage *nNmsg = check_and_cast<SimpleMessage *>(msg);
    set<int> B,U,W;

    B = nNmsg->getBlack();
    U = nNmsg->getUnknown();
    W = nNmsg->getWhite();

    set<int> NB;
    std::set_union(Black_nodes.begin(),Black_nodes.end(),
                        B.begin(),B.end(),
                        std::inserter(NB,NB.begin()));

    Black_nodes = NB;

    set<int> NW;
    std::set_union(White_nodes.begin(),White_nodes.end(),
                        W.begin(),W.end(),
                        std::inserter(NW,NW.begin()));

    White_nodes = NW;

    set<int> NU;
    std::set_union(Unknown.begin(),Unknown.end(),
                        U.begin(),U.end(),
                        std::inserter(NU,NU.begin()));

    set<int> NUnk;
    std::set_difference(NU.begin(),NU.end(),
                      Black_nodes.begin(),Black_nodes.end(),
                      std::inserter(NUnk, NUnk.begin()));

    set<int> FU;
    std::set_difference(NUnk.begin(),NUnk.end(),
                      White_nodes.begin(),White_nodes.end(),
                      std::inserter(FU, FU.begin()));
    Unknown = FU;

    if(!done){

        if(Unknown.empty()){
            done = true;
            idle = false;
        }

        for(int i =0; i < gateSize("gate$o"); i++){
                SimpleMessage *nmsg = new SimpleMessage("Message");
                nmsg->setBlack(Black_nodes);
                nmsg->setWhite(White_nodes);
                nmsg->setUnknown(Unknown);

                send(nmsg,"gate$o",i);
            }
    }
    NumBlack = Black_nodes.size();
}

void Computer::finish(){
    EV << "===============  === Report ===  ===============\n";

       // This function is called by OMNeT++ at the end of the simulation.
       EV << "          Ma couleur est :           " ;
       if (Macouleur == 0){
           EV << " Noir " << endl;
       }
       else{
           EV << " Blanc " << endl;
       }
       EV << "---------- les noeud noires sont -------------   "<< endl;
       set<int>::iterator it ;
        for (std::set<int>::iterator it=Black_nodes.begin(); it!=Black_nodes.end(); ++it)
          EV << ' ' << *it;
        EV<<endl;
        EV << "---------- les noeud blanc sont -------------   "<< endl;
        for (std::set<int>::iterator it=White_nodes.begin(); it!=White_nodes.end(); ++it)
                  EV << ' ' << *it;
        EV<<endl;
       EV << "===============  === ====== ===  ===============\n";
}












