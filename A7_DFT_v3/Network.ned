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

// Created: 2021-11-3 for project A7_DFT_v3

import Computer;

import ned.DelayChannel;

//
// A generated network with full graph topology.
//
network Network
{
types:
channel Channel extends ned.DelayChannel {
        delay = 100ms;
}
submodules:
	Computer[6]: Computer;
connections:
    Computer[0].gate++ <--> Channel <--> Computer[1].gate++;
    Computer[1].gate++ <--> Channel <--> Computer[2].gate++;
    Computer[1].gate++ <--> Channel <--> Computer[4].gate++;
    Computer[3].gate++ <--> Channel <--> Computer[4].gate++;
    Computer[4].gate++ <--> Channel <--> Computer[5].gate++;
}