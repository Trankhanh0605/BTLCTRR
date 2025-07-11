#include "tsm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std; 


std::string Traveling1(int G[][3], int num_edges, char start) {
    // Bước 1: Thu thập các đỉnh duy nhất
    std::vector<char> vertices;
    for (int i = 0; i < num_edges; i++) {
        char u = static_cast<char>(G[i][0]);
        char v = static_cast<char>(G[i][1]);
        if (std::find(vertices.begin(), vertices.end(), u) == vertices.end()) {
            vertices.push_back(u);
        }
        if (std::find(vertices.begin(), vertices.end(), v) == vertices.end()) {
            vertices.push_back(v);
        }
    }
    std::sort(vertices.begin(), vertices.end());
    int n = vertices.size();

    // Bước 2: Ánh xạ đỉnh sang chỉ số
    std::vector<std::pair<char, int>> index_map;
    for (int i = 0; i < n; i++) {
        index_map.push_back(std::make_pair(vertices[i], i));
    }

    // Tìm chỉ số của đỉnh xuất phát
    int start_idx = -1;
    for (const auto& p : index_map) {
        if (p.first == start) {
            start_idx = p.second;
            break;
        }
    }
    if (start_idx == -1) {
        return "Error: Start vertex not found";
    }

    // Bước 3: Khởi tạo ma trận đồ thị
    const int INF = 99999999;
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, INF));
    for (int i = 0; i < n; i++) {
        graph[i][i] = 0; // Khoảng cách từ đỉnh đến chính nó là 0
    }
    for (int i = 0; i < num_edges; i++) {
        char u_char = static_cast<char>(G[i][0]);
        char v_char = static_cast<char>(G[i][1]);
        int u = -1, v = -1;
        for (const auto& p : index_map) {
            if (p.first == u_char) u = p.second;
            if (p.first == v_char) v = p.second;
        }
        if (u != -1 && v != -1) {
            graph[u][v] = G[i][2]; // Gán trọng số cạnh
        }
    }

    // Bước 4: Khởi tạo bảng DP và parent
    int max_mask = 1 << n;
    std::vector<std::vector<int>> dp(max_mask, std::vector<int>(n, -1));
    std::vector<std::vector<int>> parent(max_mask, std::vector<int>(n, -1));

    // Bước 5: Hàm TSP sử dụng đệ quy với lambda
    auto tsp = [&](auto&& self, int mask, int pos) -> int {
        if (mask == (1 << n) - 1) { // Đã thăm tất cả đỉnh
            return graph[pos][start_idx]; // Quay lại đỉnh xuất phát
        }
        if (dp[mask][pos] != -1) { // Đã tính trước đó
            return dp[mask][pos];
        }
        int ans = INF;
        for (int next = 0; next < n; next++) {
            if ((mask & (1 << next)) == 0 && graph[pos][next] != INF) {
                int new_mask = mask | (1 << next);
                int cost = graph[pos][next] + self(self, new_mask, next);
                if (cost < ans) {
                    ans = cost;
                    parent[mask][pos] = next; // Lưu đỉnh tiếp theo
                }
            }
        }
        dp[mask][pos] = ans;
        return ans;
    };

    // Bước 6: Tính chi phí tối thiểu
    int min_cost = tsp(tsp, 1 << start_idx, start_idx);

    // Bước 7: Xây dựng đường đi
    std::string path;
    int mask = 1 << start_idx;
    int pos = start_idx;
    while (true) {
        path += vertices[pos];
        path += " ";
        int next = parent[mask][pos];
        if (next == -1) break;
        mask |= (1 << next);
        pos = next;
    }
    path += start; // Quay lại đỉnh xuất phát

    return path;
}




// Constants for ACO
const int MAX_ITERATIONS = 100;
const int NUM_ANTS = 10;
const double ALPHA = 1.0;
const double BETA = 2.0;
const double RHO = 0.1;
const double Q = 100.0;
const double TAU0 = 1.0;

// Simple random number generator
double simpleRand(int& seed) {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return static_cast<double>(seed) / 0x7fffffff;
}

// Get distance between two vertices
double getDistance(char u, char v, int edgeList[][3], int numEdge) {
    for (int i = 0; i < numEdge; ++i) {
        if (static_cast<char>(edgeList[i][0]) == u && static_cast<char>(edgeList[i][1]) == v) {
            return edgeList[i][2];
        }
    }
    return 1e9; // Large value if no edge exists
}

// Select next vertex based on probability
char selectNextVertex(char current, const std::vector<char>& unvisited,
                      const std::vector<std::pair<std::pair<char, char>, double>>& pheromone,
                      int edgeList[][3], int numEdge, int& seed) {
    double total = 0.0;
    std::vector<std::pair<char, double>> probabilities;

    for (char next : unvisited) {
        for (const auto& p : pheromone) {
            if (p.first.first == current && p.first.second == next) {
                double tau = p.second;
                double eta = 1.0 / getDistance(current, next, edgeList, numEdge);
                double value = pow(tau, ALPHA) * pow(eta, BETA);
                probabilities.push_back({next, value});
                total += value;
                break;
            }
        }
    }

    if (total == 0.0 && !unvisited.empty()) return unvisited[0];

    double r = simpleRand(seed) * total;
    double sum = 0.0;
    for (const auto& prob : probabilities) {
        sum += prob.second;
        if (sum >= r) return prob.first;
    }
    return unvisited[0];
}

// Main ACO function
std::string Traveling2(int edgeList[][3], int numEdge, char start) {
    int seed = 123456789; // Fixed seed for pseudo-randomness

    // Step 1: Identify all vertices
    std::vector<char> vertices;
    for (int i = 0; i < numEdge; ++i) {
        char u = static_cast<char>(edgeList[i][0]);
        char v = static_cast<char>(edgeList[i][1]);
        if (std::find(vertices.begin(), vertices.end(), u) == vertices.end()) vertices.push_back(u);
        if (std::find(vertices.begin(), vertices.end(), v) == vertices.end()) vertices.push_back(v);
    }
    int numVertices = vertices.size();

    // Step 2: Initialize pheromone for existing edges
    std::vector<std::pair<std::pair<char, char>, double>> pheromone;
    for (int i = 0; i < numEdge; ++i) {
        char u = static_cast<char>(edgeList[i][0]);
        char v = static_cast<char>(edgeList[i][1]);
        pheromone.push_back({{u, v}, TAU0});
    }

    std::vector<char> bestPath;
    double bestLength = 1e9;

    // Step 3: ACO iterations
    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        std::vector<std::vector<char>> antPaths(NUM_ANTS);
        std::vector<double> antLengths(NUM_ANTS, 0.0);

        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            std::vector<char>& path = antPaths[ant];
            std::vector<char> unvisited = vertices;
            char current = start;
            path.push_back(current);
            unvisited.erase(std::find(unvisited.begin(), unvisited.end(), current));

            // Build path
            while (!unvisited.empty()) {
                char next = selectNextVertex(current, unvisited, pheromone, edgeList, numEdge, seed);
                path.push_back(next);
                antLengths[ant] += getDistance(current, next, edgeList, numEdge);
                current = next;
                unvisited.erase(std::find(unvisited.begin(), unvisited.end(), next));
            }
            // Return to start
            path.push_back(start);
            antLengths[ant] += getDistance(current, start, edgeList, numEdge);

            // Update best path
            if (antLengths[ant] < bestLength) {
                bestLength = antLengths[ant];
                bestPath = path;
            }
        }

        // Update pheromone
        for (auto& p : pheromone) {
            p.second *= (1.0 - RHO); // Evaporation
        }
        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            for (size_t i = 0; i < antPaths[ant].size() - 1; ++i) {
                char u = antPaths[ant][i];
                char v = antPaths[ant][i + 1];
                for (auto& p : pheromone) {
                    if (p.first.first == u && p.first.second == v) {
                        p.second += Q / antLengths[ant];
                        break;
                    }
                }
            }
        }
    }

    // Step 4: Construct the result string
    std::string result;
    for (size_t i = 0; i < bestPath.size(); ++i) {
        result += bestPath[i];
        if (i < bestPath.size() - 1) result += " ";
    }
    return result;
}










// Wrapper that selects the appropriate TSP algorithm based on the number of vertices
std::string Traveling(int G[][3], int num_edges, char start) {
    bool seen[256] = {false};
    int count = 0;
    for (int i = 0; i < num_edges; ++i) {
        unsigned char u = static_cast<unsigned char>(G[i][0]);
        unsigned char v = static_cast<unsigned char>(G[i][1]);
        if (!seen[u]) {
            seen[u] = true;
            ++count;
        }
        if (!seen[v]) {
            seen[v] = true;
            ++count;
        }
    }
    int n = count;

    // Use exact DP (Traveling1) for up to 20 vertices, otherwise use ACO (Traveling2)
    if (n <= 20) {
        return Traveling1(G, num_edges, start);
    } else {
        return Traveling2(G, num_edges, start);
    }
}

