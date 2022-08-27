#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#define ll long long
#define endl "\n"

struct vertex{
    int v;
    ll weight;

    vertex(int v, ll weight){
        this->v = v;
        this->weight = weight;
    }
};
inline bool operator<(const vertex& v1, const vertex& v2) {
    if(v1.weight != v2.weight) return v1.weight < v2.weight;
    return v1.v < v2.v;
}

void print_path(int* parent, int start, int end){
    std::stack<int> s;
    int curr = end;
    while(true){
        s.push(curr);
        if(curr == start) break;
        curr = parent[curr];
    }
    while(!s.empty()){
        std::cout<<s.top();
        s.pop();
        if(!s.empty()) std::cout<<" -> ";
    }
    std::cout<<endl;
}

ll dijkstra(int n, std::vector<int> edgeList[], std::vector<int> weightList[], int* parent, int start, int end){
    ll* distance = new ll[n];
    // bool* taken = new bool[n];
    for(int i=0; i<n; i++){
        parent[i] = -1;
        distance[i] = -1;
        // taken[i] = false;
    }
    std::set<vertex> q;
    distance[start] = 0;
    q.insert(vertex(start, 0));

    while(!q.empty()){
        vertex currNode = *q.begin();
        q.erase(currNode);
        int u = currNode.v;
        // taken[u] = true;
        
        for(int i=0; i<edgeList[u].size(); i++){
            int v = edgeList[u][i];
            ll w = weightList[u][i];
            // if(taken[v]) continue;

            if(distance[v]==-1 || distance[u]+w < distance[v]){
                if(distance[v] != -1) q.erase(vertex(v, distance[v]));
                distance[v] = distance[u] + w;
                parent[v] = u;
                q.insert(vertex(v, distance[v]));
            }
        }
    }

    ll result = distance[end];
    // delete[] taken;
    delete[] distance;
    return result;
}

int main(){
    // freopen("in.txt", "r", stdin);
    int n, m, u, v;
    ll w;
    std::cin>>n>>m;
    std::vector<int>* edgeList = new std::vector<int>[n];
    std::vector<int>* weightList = new std::vector<int>[n];
    int* parent = new int[n];

    for(int i=0; i<m; i++){
        std::cin>>u>>v>>w;
        edgeList[u].push_back(v);
        weightList[u].push_back(w);
    }

    std::cin>>u>>v;
    ll result = dijkstra(n, edgeList, weightList, parent, u, v);
    if(result == -1) std::cout<<"The destination is unreachable from the source"<<endl;
    else{
        std::cout<<"Shortest path cost: "<<result<<endl;
        print_path(parent, u, v);
    }

    delete[] edgeList;
    delete[] weightList;
    delete[] parent;
    return 0;
}