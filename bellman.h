#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

//khai bao va dinh nghia ham BF
void BF(int edgeList[][3], int numberOfEdges, char startVertex, int BFvalue[], int BFPrev[]); 

//khai bao va dinh nghia ham BF_Path
std::string BF_Path(int edges[][3], int m, char startV, char endV);

#endif // BELLMAN_H
