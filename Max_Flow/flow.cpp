#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <string>
#define UNDEFINED -1
#define INF 2000000000

int bfs_flow(int n, int s, int t, int **residual, int parent[]){
    int *min = new int[n];
    for(int i=0; i<n; i++){
        min[i] = UNDEFINED;
        parent[i] = UNDEFINED;
    }

    std::queue<int> q;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        if(u==t) break;
        for(int v=0; v<n; v++){
            if(parent[v] != UNDEFINED || v==u || residual[u][v] <= 0) continue;
            q.push(v);
            parent[v] = u;
            if(u==s) min[v] = residual[u][v];
            else min[v] = std::min(min[u], residual[u][v]);
        }
    }

    int result = min[t];
    delete[] min;
    return result;
}

void dfs(int n, int **residual, bool visited[], int curr){
    if(visited[curr]) return;
    visited[curr] = true;
    for(int next=0; next<n; next++){
        if(!visited[next] && residual[curr][next] > 0) dfs(n, residual, visited, next);
    }
}

void update(int s, int t, int **residual, int parent[], int minFlow){
    int v = t;
    while(v != s){
        int u = parent[v];
        residual[u][v] -= minFlow;
        residual[v][u] += minFlow;
        v = parent[v];
    }
}

int edmonds_karp(int n, int s, int t, int **residual){
    int *parent = new int[n];
    int totalFlow = 0;
    while(true){
        int minFlow = bfs_flow(n, s, t, residual, parent);
        if(minFlow == UNDEFINED) break;
        totalFlow += minFlow;
        update(s, t, residual, parent, minFlow);
    }
    delete[] parent;
    return totalFlow;
}

void min_cut(int real_n, int n, int s, int t, int baseline, int** residual, std::vector<int>& cut_source){
    bool *visited = new bool[n];
    for(int i=0; i<n; i++) visited[i] = false;
    dfs(n, residual, visited, s);

    for(int i=0; i<real_n; i++){
        if(visited[baseline+i]) cut_source.push_back(i);
    }
}

int main(){
    int n;
    std::cin>>n;
    int *w = new int[n], *l = new int[n], *r = new int[n], *team_node = new int[n];
    int **original = new int*[n], **game_node = new int*[n];
    int curr_game_node = 1;
    for(int i=0; i<n; i++){
        original[i] = new int[n];
        game_node[i] = new int[n];
        team_node[i] = 1 + i + n*(n-1)/2;
        for(int j=0; j<n; j++){
            original[i][j] = 0;
            if(j > i) game_node[i][j] = curr_game_node++;
        }
    }
    
    std::string *name = new std::string[n];
    for(int i=0; i<n; i++){
        std::cin>>name[i]>>w[i]>>l[i]>>r[i];
        for(int j=0; j<n; j++){
            int w;
            std::cin>>w;
            original[i][j] = w;
        }
    }

    int max_winner_index = 0;
    for(int i=1; i<n; i++){
        if(w[i] > w[max_winner_index]) max_winner_index = i;
    }

    int n_new = n + n*(n-1)/2 + 2, s = 0, t = n_new-1;
    int **residue = new int*[n_new];
    for(int i=0; i<n_new; i++)
        residue[i] = new int[n_new];

    for(int x=0; x<n; x++){
        for(int i=0; i<n_new; i++)
            for(int j=0; j<n_new; j++) residue[i][j] = 0;
        
        int expected_flow = 0;
        for(int i=0; i<n; i++){
            if(i==x) continue;
            residue[team_node[i]][t] = std::max(0, w[x] + r[x] - w[i]);
            for(int j=i+1; j<n; j++){
                if(j==x) continue;
                int gn = game_node[i][j];
                residue[s][gn] = original[i][j];
                expected_flow += original[i][j];
                residue[gn][team_node[i]] = INF;
                residue[gn][team_node[j]] = INF;
            }
        }

        int ans = edmonds_karp(n_new, s, t, residue);
        if(ans < expected_flow) std::cout<<name[x]<<" is eliminated\n";
        else if(w[x] + r[x] < w[max_winner_index]){
            std::cout<<name[x]<<" is eliminated\n";
            printf("They can win at most %d + %d = %d games\n", w[x], r[x], w[x]+r[x]);
            std::cout<<name[max_winner_index]<<" has already won "<<w[max_winner_index]<<" games.\n";
            continue;
        }
        else continue;
        printf("They can win at most %d + %d = %d games\n", w[x], r[x], w[x]+r[x]);


        std::vector<int> cut_source;
        min_cut(n, n_new, s, t, team_node[0], residue, cut_source);
        int total_win = 0, total_game = 0;

        for(int i=0; i<cut_source.size(); i++){
            std::cout<<name[cut_source[i]];
            total_win += w[cut_source[i]];
            for(int j=i+1; j<cut_source.size(); j++){
                total_game += original[cut_source[i]][cut_source[j]];
            }
            if(i != cut_source.size()-1) std::cout<<", ";
            if(i == cut_source.size()-2) std::cout<<"and ";
        }
        std::cout<<" have won a total of "<<total_win<<" games\n";
        std::cout<<"They play each other "<<total_game<<" times\n";
        printf("So on average, each of the teams wins %d/%d = %0.2lf games.\n", 
                total_win+total_game, cut_source.size(), 1.00*(total_win+total_game)/cut_source.size());
    }

    for(int i=0; i<n_new; i++) delete[] residue[i];
    for(int i=0; i<n; i++) delete[] original[i], game_node[i];
    delete[] residue, original, game_node, w, l, r, team_node, name;
    return 0;
}