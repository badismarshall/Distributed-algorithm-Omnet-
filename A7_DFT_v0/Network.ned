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
    parameters:
        int n = default(5);
    submodules:
        node[n]: Computer {
            @display("p=,,ring");
            
        }
    connections:
        for i=0..n-2, for j=i+1..n-1 {
            node[i].gate++ <--> DelayChannel <--> node[j].gate++;
        }
}
