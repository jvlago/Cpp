#include <iostream>
#include <vector>
#include <algorithm>

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DisjointSet {
    std::vector<int> parent, rank;
public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if (xr == yr) return false;
        if (rank[xr] < rank[yr]) parent[xr] = yr;
        else if (rank[xr] > rank[yr]) parent[yr] = xr;
        else parent[yr] = xr; rank[xr]++;
        return true;
    }
};

std::vector<Edge> kruskal(int n, std::vector<Edge>& edges) {
    std::sort(edges.begin(), edges.end());
    DisjointSet ds(n);
    std::vector<Edge> mst;
    for (const auto& e : edges) {
        if (ds.unite(e.u, e.v)) mst.push_back(e);
    }
    return mst;
}

// Example usage:
int main() {
    int n = 4; // number of vertices
    std::vector<Edge> edges = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5}, {1, 3, 15}, {2, 3, 4}
    };
    auto mst = kruskal(n, edges);
    int total = 0;
    std::cout << "Edges in MST:\n";
    for (const auto& e : mst) {
        std::cout << e.u << " - " << e.v << " : " << e.weight << "\n";
        total += e.weight;
    }
    std::cout << "Total weight: " << total << std::endl;
    return 0;
}