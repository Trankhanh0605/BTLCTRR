
#include "bellman.h"

using namespace std; 


void BF(int edgeList[][3], int numberOfEdges, char startVertex, int BFvalue[], int BFPrev[]) {
    const int INF = 100000000;

    struct Edge {
        int u, v, weight;
    };

    // 1) Thu thập và sort các đỉnh, map char -> index
    vector<int> verts;
    verts.reserve(numberOfEdges*2);
    for(int i=0;i<numberOfEdges;i++){
        verts.push_back(edgeList[i][0]);
        verts.push_back(edgeList[i][1]);
    }
    sort(verts.begin(), verts.end());
    verts.erase(unique(verts.begin(), verts.end()), verts.end());
    int V = verts.size();
    vector<int> charToIndex(256, -1);
    for(int i=0;i<V;i++) charToIndex[verts[i]] = i;

    // 2) Khởi tạo BFvalue & BFPrev
    int si = charToIndex[(unsigned char)startVertex];
    BFvalue[si] = 0;
    for(int i=0;i<V;i++){
        if(i!=si && BFvalue[i]==-1) 
            BFvalue[i] = INF;
    }
    // Lưu BFPrev[si] = -1 (mặc định bên ngoài đã set toàn -1)

    // 3) Build edge list với index
    vector<Edge> edges;
    edges.reserve(numberOfEdges);
    for(int i=0;i<numberOfEdges;i++){
        int u = charToIndex[(unsigned char)edgeList[i][0]];
        int v = charToIndex[(unsigned char)edgeList[i][1]];
        int w = edgeList[i][2];
        if(u>=0 && v>=0) edges.push_back({u,v,w});
    }
    // sort theo u để xác định thứ tự relax
    sort(edges.begin(), edges.end(), [](auto &a, auto &b){
        return a.u < b.u;
    });

    // 4) Relax một bước (dùng bản copy để không override ngay)
    vector<int> oldV(BFvalue, BFvalue+V),
                newV = oldV,
                newP(BFPrev, BFPrev+V);
    for(auto &e: edges){
        if(oldV[e.u] != INF){
            int cand = oldV[e.u] + e.weight;
            if(newV[e.v]==INF || cand < newV[e.v]) {
                newV[e.v] = cand;
                newP[e.v] = e.u;
            }
        }
    }
    // commit kết quả
    for(int i=0;i<V;i++){
        BFvalue[i] = newV[i];
        BFPrev[i]  = newP[i];
    }

    // 5) Cuối cùng: chuyển tất cả INF trở lại -1 để in ra đúng định dạng
    for(int i=0;i<V;i++){
        if(BFvalue[i] == INF) {
            BFvalue[i] = -1;
            // BFPrev[i] để -1 luôn
        }
    }
}

string BF_Path(int edges[][3], int m, char startV, char endV) {
    // Bước 1: Thu thập và loại bỏ trùng lặp các đỉnh
    std::vector<char> vertices;
    for (int i = 0; i < m; ++i) {
        vertices.push_back(static_cast<char>(edges[i][0]));
        vertices.push_back(static_cast<char>(edges[i][1]));
    }
    std::sort(vertices.begin(), vertices.end());
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    int n = vertices.size();

    // Bước 2: Tạo bảng ánh xạ từ ký tự sang chỉ số
    std::vector<int> charToIndex(256, -1);
    for (int i = 0; i < n; ++i) {
        charToIndex[static_cast<unsigned char>(vertices[i])] = i;
    }

    // Bước 3: Khởi tạo mảng BFvalue và BFPrev
    std::vector<int> BFvalue(n, -1);
    std::vector<int> BFPrev(n, -1);

    // Bước 4: Chạy Bellman-Ford (n-1) lần
    for (int i = 0; i < n - 1; ++i) {
        BF(edges, m, startV, BFvalue.data(), BFPrev.data());
    }

    // Bước 5: Truy vết đường đi từ endV về startV
    int endIdx = charToIndex[static_cast<unsigned char>(endV)];
    if (BFvalue[endIdx] == -1) {
        return "No path"; // Không có đường đi
    }

    std::string path;
    std::vector<char> reversePath;
    int current = endIdx;
    while (current != -1) {
        reversePath.push_back(vertices[current]);
        current = BFPrev[current];
    }

    // Bước 6: Đảo ngược và định dạng đường đi thành chuỗi
    std::reverse(reversePath.begin(), reversePath.end());
    for (size_t i = 0; i < reversePath.size(); ++i) {
        path += reversePath[i];
        if (i + 1 < reversePath.size()) {
            path += " ";
        }
    }

    return path;
}







