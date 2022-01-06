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

// Created: 2021-10-24 for project A4_diffuse-v3_with_set

import Computer;

import ned.DelayChannel;

//
// A generated network with torus topology.
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
	Computer[4].gate++ <-->  Computer[1].gate++;
	Computer[1].gate++ <-->  Computer[5].gate++;
	Computer[6].gate++ <-->  Computer[5].gate++;
	Computer[6].gate++ <-->  Computer[5].gate++;
	Computer[3].gate++ <-->  Computer[6].gate++;
}

  network BTree {

        int height;
    types:
	   channel Channel extends ned.DelayChannel {
	   	delay = 100 ms;
	    }
    submodules:
       	Computer[2^height+1] : Computer;
    connections:
        
        for i = 0..2^(height-1)-1 {
              Computer[i].gate++ <--> Channel <--> Computer[2*i+1].gate++;
              Computer[i].gate++ <--> Channel <-->  Computer[2*i+2].gate++;
        }	
  }

  network HyperCube {
   int dim;
    
    types:
	   channel Channel extends ned.DelayChannel {
	   	delay = 100 ms;
	    }
    submodules:
       	Computer[2^dim] : Computer;
    connections:
        
        for i = 0..2^dim-1, for j=0..dim-1 {
            Computer[i].gate++ <--> Channel <--> Computer[i # 2^j].gate++ ;
        }	
  }
  
  network Random {
	int n;
    types:
    	channel Channel extends ned.DelayChannel {
    	   	delay = 100 ms;
    	}
    submodules:
       	Computer[n] : Computer;
    connections:
        
        for i = 0..n-1 , for j = 0..n-1 {
            Computer[i].gate++ <--> Channel <-->  Computer[j].gate++ if i!=j &&  intuniform(0,1) == 1;
        }	
  }