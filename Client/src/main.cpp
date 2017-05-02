// Author: Jonas Knarbakk (20:18:36-26/04/2017)
// Description: New project

#include <iostream>
#include <cstdlib>
#include "connection.hpp"

int main(){
    Connection con("localhost", 7145);
    con.sendToServer("Hello!");
} 
