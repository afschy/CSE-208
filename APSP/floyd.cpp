#include <iostream>
#include <algorithm>
#define endl "\n"
#define INF 2000000000

void print_path(int** next, int start, int end){
    int curr = start;
    while(true){
        std::cout<<curr;
        if(curr==end) break;
        std::cout<<" -> ";
        curr = next[curr][end];
    }
    std::cout<<endl;
}

void floyd_warshall(int n, int** w, int** next){
    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(w[i][k]==INF || w[k][j]==INF) continue;
                if(w[i][k] + w[k][j] < w[i][j]){
                    w[i][j] = w[i][k] + w[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

int main(){
    // freopen("in.txt", "r", stdin);
    int n, m, u, v, wt;
    std::cin>>n>>m;
    int **w = new int*[n], **next = new int*[n];
    
    for(int i=0; i<n; i++){
        w[i] = new int[n]; next[i] = new int[n];
        for(int j=0; j<n; j++){
            w[i][j] = INF;
            next[i][j] = j;
        }
        w[i][i] = 0;
    }

    for(int i=0; i<m; i++){
        std::cin>>u>>v>>wt;
        w[u-1][v-1] = wt;
    }

    floyd_warshall(n, w, next);

    std::cout<<"Shortest distance matrix"<<endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(w[i][j] == INF) std::cout<<"INF ";
            else std::cout<<w[i][j]<<" ";
        }
        std::cout<<endl;
    }

    for(int i=0; i<n; i++){
        delete[] w[i];
        delete[] next[i];
    }
    delete[] w;
    delete[] next;

    return 0;
}