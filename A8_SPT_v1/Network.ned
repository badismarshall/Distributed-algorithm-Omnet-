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

// Created: 2021-11-4 for project A8_SPT_v1

import Computer;

import ned.DelayChannel;

//
// A generated network with ring topology.
//
network Network
{

submodules:
	Computer[7]: Computer;
connections:
    Computer[0].gate++ <-->  Computer[1].gate++;
    Computer[1].gate++ <-->  Computer[2].gate++;
    Computer[1].gate++ <-->  Computer[4].gate++;
    Computer[3].gate++ <-->  Computer[4].gate++;
    Computer[4].gate++ <-->  Computer[5].gate++;
	Computer[1].gate++ <-->  Computer[5].gate++;
	Computer[6].gate++ <-->  Computer[5].gate++;
	Computer[3].gate++ <-->  Computer[6].gate++;
}
