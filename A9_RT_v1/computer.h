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
#include "SimpleMessage_m.h"
#include <omnetpp.h>
#include <map>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Computer : public cSimpleModule
{
  public:
    std::set<int> Vos;
    std::map<int,int> Vos2;
    int counter;
    cHistogram hopCountStats;
    cOutVector hopCountVector;
    long numSent;
    long numReceived;
    bool intt;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    virtual void forwardMessage();
};

#endif
