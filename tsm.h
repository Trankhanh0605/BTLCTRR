#ifndef TSM_H
#define TSM_H


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

//khai bao va dinh nghia ham Traveling cho 15-20 dinh 
std::string Traveling1(int G[][3], int num_edges, char start);
//khai bao va dinh nghia ham Traveling cho 20-30 dinh
std::string Traveling2(int G[][3], int num_edges, char start);

//khai bao va dinh nghia ham Traveling goi den Traveling1 (15-20 đỉnh) hoac Traveling2 (20-30 đỉnh)
std::string Traveling(int G[][3], int num_edges, char start);

#endif // TSM_H
