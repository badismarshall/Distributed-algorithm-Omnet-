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

cplusplus {{
       #include <set>
       typedef std::set<int> set;
}};
class noncobject set;
message SimpleMessage
{
int source;
int destiontion;
int kind; // kind = 1 if msg is token // kind = 2 if msg is return // kind = 3 if msg is backedge
int count;
}