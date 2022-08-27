#include <iostream>
#include <algorithm>
#define endl "\n"
#define INF 2000000000

void extend_shortest_path(int n, int** l){
    int** l_old = new int*[n];
    for(int i=0; i<n; i++) l_old[i] = new int[n];

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            l_old[i][j] = l[i][j];

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            l[i][j] = INF;
            for(int k=0; k<n; k++){
                if(l_old[i][k]==INF || l_old[k][j]==INF) continue;
                if(l_old[i][k]+l_old[k][j] < l[i][j]){
                    l[i][j] = l_old[i][k] + l_old[k][j];
                }
            }
        }
    }

    for(int i=0; i<n; i++) delete[] l_old[i];
    delete[] l_old;
}

void apsp_matrix(int n, int** w){
    int m = 1;
    for(int m=1; m<=n-1; m*=2)
        extend_shortest_path(n, w);
}

int main(){
    // freopen("in.txt", "r", stdin);
    int n, m, u, v, weight;
    std::cin>>n>>m;
    int** w = new int*[n];
    int** l = new int*[n];
    for(int i=0; i<n; i++){
        w[i] = new int[n];
        l[i] = new int[n];
        for(int j=0; j<n; j++){
            w[i][j] = INF;
        }
        w[i][i] = 0;
    }

    for(int i=0; i<m; i++){
        std::cin>>u>>v>>weight;
        w[u-1][v-1] = weight;
    }
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            l[i][j] = w[i][j];
    
    apsp_matrix(n, l);
    
    std::cout<<"Shortest distance matrix"<<endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(l[i][j] == INF) std::cout<<"INF ";
            else std::cout<<l[i][j]<<" ";
        }
        std::cout<<endl;
    }

    for(int i=0; i<n; i++){ 
        delete[] w[i];
        delete[] l[i];
    }
    delete[] w;
    delete[] l;
}