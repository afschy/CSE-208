#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#define ll long long
#define endl "\n"

struct edge {
    int u;
    int v;
    ll weight;

    edge(int u1, int v1, ll w1){
        u = u1;
        v = v1;
        weight = w1;
    }
};

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

void dfs(std::vector<int> adjList[], bool* visited, int curr){
    visited[curr] = true;
    for(int next: adjList[curr]){
        if(!visited[next]) dfs(adjList, visited, next);
    }
}

struct result_container{
    ll dist;
    bool isCycle;
    bool isValidPath;

    result_container(ll d=0, bool c=false, bool v=false){
        dist = d;
        isCycle = c;
        isValidPath = v;
    }
};

result_container bellman_ford(int n, std::vector<edge> edgeList, std::vector<int>* adjList, int* parent, int start, int end){
    ll* distance = new ll[n];
    int m = edgeList.size();
    for(int i=0; i<n; i++){
        distance[i] = INT_MAX;
    }
    distance[start] = 0;
    
    for(int k=1; k<=n-1; k++){
        for(int i=0; i<m; i++){
            int u = edgeList[i].u, v = edgeList[i].v;
            ll w = edgeList[i].weight;
            if(distance[u] == INT_MAX) continue;
            if(distance[u] + w < distance[v]){
                distance[v] = distance[u] + w;
                parent[v] = u;
            }
        }
    }

    bool* visited = new bool[n];
    bool cycle = false;
    for(int i=0; i<n; i++) visited[i] = false;
    for (int i=0; i<m; i++){
        if (distance[edgeList[i].u] + edgeList[i].weight < distance[edgeList[i].v] && !visited[edgeList[i].u]){
            dfs(adjList, visited, edgeList[i].u);
            cycle = true;
        }
    }

    result_container result;
    if(!cycle){
        result.isCycle = false;
        if(distance[end] != INT_MAX){
            result.dist = distance[end];
            result.isValidPath = true;
        }
        else
            result.isValidPath = false;
    }

    else{
        result.isCycle = true;
        if(visited[end]){
            result.isValidPath = false;
        }
        else if(distance[end] != INT_MAX){
            result.dist = distance[end];
            result.isValidPath = true;
        }
        else
            result.isValidPath = false;
    }

    delete[] distance;
    delete[] visited;
    return result;
}

int main(){
    // freopen("in.txt", "r", stdin);
    int n, m, u, v;
    ll w;
    std::cin>>n>>m;
    std::vector<edge> edgeList;
    std::vector<int>* adjList = new std::vector<int>[n];
    int* parent = new int[n];

    for(int i=0; i<m; i++){
        std::cin>>u>>v>>w;
        edgeList.push_back(edge(u, v, w));
        adjList[u].push_back(v);
    }

    std::cin>>u>>v;
    result_container result = bellman_ford(n, edgeList, adjList, parent, u, v);
    if(result.isCycle){
        std::cout<<"The graph contains a negative cycle"<<endl;
        if(result.isValidPath){
            std::cout<<"Shortest path cost: "<<result.dist<<endl;
            print_path(parent, u, v);
        }
    }
    else{
        std::cout<<"The graph does not contain a negative cycle that is reachable from the source"<<endl;
        if(result.isValidPath){
            std::cout<<"Shortest path cost: "<<result.dist<<endl;
            print_path(parent, u, v);
        }
        else
            std::cout<<"The destination is unreachable from source"<<endl;
    }

    delete[] parent;
    delete[] adjList;
    return 0;
}