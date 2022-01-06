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

#ifndef __A2_COMPUTER_H_
#define __A2_COMPUTER_H_

#include <omnetpp.h>
#include "SimpleMessage_m.h"
#include<set>
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Computer : public cSimpleModule
{
  public:
    bool root;
    int counter = 0;
    int parent;
    int qs; // nombre de reponse qu'il doit recevoire le noeud
    std::set<int> St;
    std::set<int> Vos;
    bool idle = false;
    bool active =false;
    int done = false;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif