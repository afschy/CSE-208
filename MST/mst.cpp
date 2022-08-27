#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#define endl "\n"

struct edge{
    int u;
    int v;
    double weight;

    edge(int u=0, int v=0, double weight=0.0){
        this->u = u;
        this->v = v;
        this->weight = weight;
    }
};
inline bool operator<(const edge& e1, const edge& e2){
    return e1.weight > e2.weight;
}

struct vertex{
    int v;
    double weight;

    vertex(int v=0, double weight=0.0){
        this->v = v;
        this->weight = weight;
    }
};
inline bool operator<(const vertex& v1, const vertex& v2){
    if(v1.weight != v2.weight) return v1.weight < v2.weight;
    return v1.v < v2.v;
}

inline bool compare_edge(const edge& e1, const edge& e2){
    return e1.weight < e2.weight;
}

int get_ancestor(int* set, int curr){
    if(set[curr] == curr) return curr;
    set[curr] = get_ancestor(set, set[curr]);
    return set[curr];
}

double kruskal(int n, int m, edge* edgeList, std::vector<edge>& mst){
    std::sort(edgeList, edgeList+m, compare_edge);
    int* set = new int[n];
    for(int i=0; i<n; ++i)
        set[i] = i;
    
    double weight = 0.0;
    for(int i=0; i<m; ++i){
        edge currEdge = edgeList[i];
        if(get_ancestor(set, currEdge.u) == get_ancestor(set, currEdge.v)) continue;

        mst.push_back(currEdge);
        weight += currEdge.weight;
        set[get_ancestor(set, currEdge.v)] = get_ancestor(set, currEdge.u);
    }

    delete[] set;
    return weight;
}

double prim(int n, std::vector<int>* edges, std::vector<double>* weight, std::vector<edge>& mst, int start){
    std::set<vertex> q;
    double* key = new double[n];
    int* parent = new int[n];
    bool* taken = new bool[n];
    for(int i=0; i<n; ++i){
        key[i] = -1;
        parent[i] = -1;
        taken[i] = false;
    }

    key[start] = 0;
    q.insert(vertex(start, 0));
    
    double wt = 0.0;
    for(int k=0; k<n; k++){
        vertex currNode = *q.begin();
        q.erase(currNode);
        taken[currNode.v] = true;
        wt += currNode.weight;

        if(currNode.v != start)
            mst.push_back(edge(parent[currNode.v], currNode.v, currNode.weight));

        for(int i=0; i<edges[currNode.v].size(); ++i){
            int next = edges[currNode.v][i];
            double w = weight[currNode.v][i];
            if(taken[next]) continue;
            
            if(key[next] < 0 || w < key[next]){
                if(key[next]>=0) q.erase(vertex(next, key[next]));
                key[next] = w;
                parent[next] = currNode.v;
                vertex newVertex(next, w);
                q.insert(newVertex);
            }
        }
    }

    delete[] key;
    delete[] parent;
    delete[] taken;
    return wt;
}

int main(){
    freopen("mst.in", "r", stdin);
    int n, m;
    std::cin>>n>>m;
    
    edge* edgeList = new edge[m];
    std::vector<int>* edges = new std::vector<int>[n];
    std::vector<double>* weight = new std::vector<double>[n];

    for(int i=0; i<m; ++i){
        std::cin>> edgeList[i].u >> edgeList[i].v >> edgeList[i].weight;
        edges[edgeList[i].u].push_back(edgeList[i].v);
        weight[edgeList[i].u].push_back(edgeList[i].weight);
        edges[edgeList[i].v].push_back(edgeList[i].u);
        weight[edgeList[i].v].push_back(edgeList[i].weight);
    }

    std::vector<edge> mst_kruskal, mst_prim;
    double primCost = prim(n, edges, weight, mst_prim, 1), kruskalCost = kruskal(n, m, edgeList, mst_kruskal);
    std::cout<< "Cost of the minimum spanning tree : " << primCost << endl;
    // std::cout<< "Cost of the minimum spanning tree : " << kruskalCost << endl;

    std::cout<< "List of edges selected by Prim's:{";
    for(int i=0; i<n-1; ++i){
        std::cout<< "(" << mst_prim[i].u << "," << mst_prim[i].v << ")";
        if(i != n-2) std::cout<<",";
    }
    std::cout<< "}" << endl;
    
    std::cout<< "List of edges selected by Kruskal's:{";
    for(int i=0; i<n-1; ++i){
        std::cout<< "(" << mst_kruskal[i].u << "," << mst_kruskal[i].v << ")";
        if(i != n-2) std::cout<<",";
    }
    std::cout<< "}" << endl;

    delete[] edgeList;
    delete[] edges;
    delete[] weight;
    return 0;
}