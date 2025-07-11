#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "tsm.h"
#include "bellman.h"

int main() {
     // Số đỉnh cố định
    const int N = 7;

    // Ma trận trọng số đầu vào (0 = không có cạnh)
    int G1[N][N] = {
        {0, 5, 0, 14, 0, 0, 0},
        {5, 0, 8, 0, 0, 3, 0},
        {0, 8, 0, 16, 0, 5, 4},
        {14, 0, 16, 0, 10, 4, 0},
        {0, 0, 0, 10, 0, 6, 7},
        {0, 3, 5, 4, 6, 0, 2},
        {0, 0, 4, 0, 7, 2, 0}
    };

    // Đỉnh bắt đầu (ký tự)
    char start = 'A';

    // --- Tạo danh sách cạnh cho TSP và BF ---
    // Max cạnh = N*(N-1)/2
    int edgeList[N*N][3];
    int numberOfEdges = 0;
    // Bảng chuyển chỉ số 0..N-1 sang 'A'..'G'
    char vertex[N];
    for (int i = 0; i < N; ++i) {
        vertex[i] = static_cast<char>('A' + i);
    }

    for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) { // Duyệt qua tất cả cặp (i,j)
        if (G1[i][j] != 0) {
            edgeList[numberOfEdges][0] = vertex[i];
            edgeList[numberOfEdges][1] = vertex[j];
            edgeList[numberOfEdges][2] = G1[i][j];
            ++numberOfEdges;
        }
    }
}
    // --- Gọi TSP ---
    std::string tsp_path = Traveling(edgeList, numberOfEdges, start);
    std::cout << "TSP path: " << tsp_path << std::endl;

    // --- Chuẩn bị cho Bellman–Ford ---
    int BFvalue[N];
    int BFprev[N];
    // Khởi tạo: start=0, các đỉnh khác = -1
    for (int i = 0; i < N; ++i) {
        BFvalue[i] = (vertex[i] == start ? 0 : -1);
        BFprev[i] = -1;
    }

    // Chạy relax Bellman–Ford N-1 lần
    for (int iter = 0; iter < N - 1; ++iter) {
        BF(edgeList, numberOfEdges, start, BFvalue, BFprev);
    }

    // In kết quả BF
    std::cout << "Bellman–Ford distances from " << start << ":\n";
    for (int i = 0; i < N; ++i) {
        std::cout << "  to " << vertex[i] << " = ";
        if (BFvalue[i] < 0) std::cout << "INF";
        else std::cout << BFvalue[i];
        std::cout << "\n";
    }

    std::cout << "Bellman–Ford paths:\n";
    for (int i = 0; i < N; ++i) {
        std::cout << "  path to " << vertex[i] << ": ";
        std::string path = BF_Path(edgeList, numberOfEdges, start, vertex[i]);
        std::cout << path << std::endl;
    }

    return 0;
}

