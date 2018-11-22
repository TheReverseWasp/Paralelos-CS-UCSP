#ifndef HELPERFUNS_H
#define HELPERFUNS_H

#include <iostream>
#include <vector>

using namespace std;

void vectoarr(vector <unsigned char> sender, unsigned char* recv) {
  for (unsigned int i = 0; i < sender.size(); i++) {
    recv[i] = sender[i];
  }
}

void arrtovec(unsigned char *sender, vector <unsigned char> &recv) {
  for (unsigned int i = 0; i < recv.size(); i++) {
    recv[i] = sender[i];
  }
}


#endif
