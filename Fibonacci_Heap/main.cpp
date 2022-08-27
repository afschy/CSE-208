#include "fibheap.cpp"
#include <set>
#define TEST_NUM 1

struct vertex{
    int v;
    ll weight;

    vertex(int v, int weight){
        this->v = v;
        this->weight = weight;
    }
};
inline bool operator<(const vertex& v1, const vertex& v2) {
    if(v1.weight != v2.weight) return v1.weight > v2.weight;
    return v1.v > v2.v;
}

// ll dijkstra_bin(int n, std::vector<int> edgeList[], std::vector<ll> weightList[], int* parent, int start, int end){
//     ll* distance = new ll[n];
//     for(int i=0; i<n; i++){
//         parent[i] = -1;
//         distance[i] = -1;
//     }
//     std::set<vertex> q;
//     distance[start] = 0;
//     q.insert(vertex(start, 0));

//     while(!q.empty()){
//         vertex currNode = *q.begin();
//         q.erase(currNode);
//         int u = currNode.v;
        
//         for(int i=0; i<edgeList[u].size(); i++){
//             int v = edgeList[u][i];
//             ll w = weightList[u][i];

//             if(distance[v]==-1 || distance[u]+w < distance[v]){
//                 if(distance[v] != -1) q.erase(vertex(v, distance[v]));
//                 distance[v] = distance[u] + w;
//                 parent[v] = u;
//                 q.insert(vertex(v, distance[v]));
//             }
//         }
//     }

//     ll result = distance[end];
//     delete[] distance;
//     return result;
// }

ll dijkstra_bin(int n, std::vector<int> edgeList[], std::vector<ll> weightList[], int* parent, int start, int end){
    ll* distance = new ll[n];
    bool* taken = new bool[n];
    for(int i=0; i<n; i++){
        parent[i] = -1;
        distance[i] = -1;
        taken[i] = false;
    }
    std::priority_queue<vertex> q;
    distance[start] = 0;
    q.push(vertex(start, 0));

    while(!q.empty()){
        vertex currNode = q.top();
        q.pop();
        int u = currNode.v;
        if(taken[u]) continue;
        taken[u] = true;
        
        for(int i=0; i<edgeList[u].size(); i++){
            int v = edgeList[u][i];
            ll w = weightList[u][i];

            if(distance[v]==-1 || distance[u]+w < distance[v]){
                distance[v] = distance[u] + w;
                parent[v] = u;
                q.push(vertex(v, distance[v]));
            }
        }
    }

    ll result = distance[end];
    delete[] distance;
    delete[] taken;
    return result;
}

ll dijkstra_fib(int n, std::vector<int> edgeList[], std::vector<ll> weightList[], int* parent, int start, int end){
    node<int,ll>** vertex = new node<int,ll>*[n];
    fibHeap<int,ll> q;
    for(int i=0; i<n; i++){
        vertex[i] = new node<int,ll>(i, INF);
        q.insert(vertex[i]);
        parent[i] = -1;
    }
    q.decrease_key(vertex[start], 0);

    while(!q.empty()){
        node<int,ll>* curr = q.extract_min();
        if(curr->key == INF) break;
        int u = curr->element;

        for(int i=0; i<edgeList[u].size(); i++){
            ll v = edgeList[u][i], w = weightList[u][i];
            if(vertex[u]->key + w < vertex[v]->key){
                q.decrease_key(vertex[v], vertex[u]->key + w);
                parent[v] = u;
            }
        }
    }

    int result = vertex[end]->key;
    for(int i=0; i<n; i++) delete vertex[i];
    delete[] vertex;
    return result;
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
        std::cout<<s.top()+1;
        s.pop();
        if(!s.empty()) std::cout<<" ";
    }
    std::cout<<endl;
}

int get_path_length(int* parent, int start, int end){
    if(start == end) return 0;
    int curr = end, count = 0;
    while(true){
        curr = parent[curr];
        count++;
        if(curr == start) break;
    }
    return count;
}

int main(){
    int n, m, u, v;
    ll w;

    //in1 and in2 - given testcase
    //in3 and in4 - very large dense graph
    //in5 and in6 - dense graph generated using tcgen
    std::string file1 = "in3.txt", file2 = "in4.txt";

    std::ifstream in1;
    in1.open(file1);
    in1>>n>>m;
    std::vector<int>* edgeList = new std::vector<int>[n];
    std::vector<ll>* weightList = new std::vector<ll>[n];
    int* parent = new int[n];
 
    for(int i=0; i<m; i++){
        in1>>u>>v>>w;
        edgeList[u].push_back(v);
        edgeList[v].push_back(u);
        weightList[u].push_back(w);
        weightList[v].push_back(w);
    }
    in1.close();

    std::ifstream in2;
    std::ofstream out;
    in2.open(file2);
    out.open("out.txt");

    int k, start, end;
    in2>>k;
    while(k--){
        in2>>start>>end;
        ll result = dijkstra_fib(n, edgeList, weightList, parent, start, end);
        out<<get_path_length(parent, start, end)<<" "<<result;

        auto start_time = std::chrono::high_resolution_clock::now();
        auto end_time = start_time;

        start_time = std::chrono::high_resolution_clock::now();
        for(int i=0; i<TEST_NUM; i++) dijkstra_bin(n, edgeList, weightList, parent, start, end);
        end_time = std::chrono::high_resolution_clock::now();
        double binTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();

        start_time = std::chrono::high_resolution_clock::now();
        for(int i=0; i<TEST_NUM; i++) dijkstra_fib(n, edgeList, weightList, parent, start, end);
        end_time = std::chrono::high_resolution_clock::now();
        double fibTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();

        out<<" "<<binTime<<" "<<fibTime<<endl;
    }

    in2.close();
    out.close();
 
    delete[] edgeList;
    delete[] weightList;
    delete[] parent;
    return 0;
}