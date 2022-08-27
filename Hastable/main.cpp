#include "generator.cpp"
#include "hashTable.cpp"
#include <chrono>
#include <fstream>

int main(){
    int N;
    std::cin>>N;
    hashTable h[4] = {hashTable(N, 1), hashTable(N, 2), hashTable(N, 3), hashTable(N, 4)};
    char** wordList = new char*[N];
    for(int i=0; i<N; i++) wordList[i] = new char[8];
    word_generator(wordList, N);
    
    double searchtime_predel[6][4] = {0.0};
    double searchtime_postdel[6][4] = {0.0};
    double probecount_predel[6][4] = {0};
    double probecount_postdel[6][4] = {0};
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time;
    
    for(int _lf=0; _lf<6; _lf++){
        double loadFactor = 0.4 + 0.1*_lf;
        int insertLimit = loadFactor * N;

        for(int i=0; i<4; i++) h[i].clear();
        for(int i=0; i<insertLimit; i++)
            for(int j=0; j<4; j++) h[j].insert(wordList[i], i+1);

        int searchLimit = insertLimit * 0.1;
        int startPoint = rand() % (insertLimit - searchLimit + 1);
        for(int i=0; i<4; i++){
            start_time = std::chrono::high_resolution_clock::now();
            for(int j=startPoint; j<startPoint + searchLimit; j++)
                h[i].get(wordList[j]);
            end_time = std::chrono::high_resolution_clock::now();

            searchtime_predel[_lf][i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time).count();
            if(i==0) continue;
            probecount_predel[_lf][i] = h[i].get_probe_count();
            h[i].set_probe_count(0);
        }

        int deleteStartPoint = insertLimit/2 - searchLimit/2;
        for(int i=0; i<4; i++)
            for(int j=deleteStartPoint; j < deleteStartPoint + searchLimit; j++)
                h[i].remove(wordList[j]);

        int delSearchLimit = searchLimit / 2;
        if(searchLimit % 2) delSearchLimit++;
        for(int i=0; i<4; i++){
            start_time = std::chrono::high_resolution_clock::now();
            for(int j=0; j<delSearchLimit; j++)
                h[i].get(wordList[j]);
            for(int j=deleteStartPoint; j < deleteStartPoint + searchLimit/2; j++)
                h[i].get(wordList[j]);
            end_time = std::chrono::high_resolution_clock::now();

            searchtime_postdel[_lf][i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time).count();
            if(i==0) continue;
            probecount_postdel[_lf][i] = h[i].get_probe_count();
            h[i].set_probe_count(0);
        }

        for(int i=0; i<4; i++){
            probecount_predel[_lf][i] /= searchLimit;
            probecount_postdel[_lf][i] /= searchLimit;
            searchtime_predel[_lf][i] /= searchLimit;
            searchtime_postdel[_lf][i] /= searchLimit;
        }
    }

    char indexName[4][20] = {"Seperate Chaining", "Linear Probing", "Quadratic Probing", "Double Hashing"};

    std::ofstream out;
    out.open("report.csv");

    out<<indexName[0]<<endl;
    out<<" ,Before deletion,After deletion"<<endl;
    out<<"Load factor,Avg search time,Avg search time"<<endl;
    for(int j=0; j<6; j++){
        out<<0.4 + 0.1*j;
        out<<","<<searchtime_predel[j][0]<<","<<searchtime_postdel[j][0]<<endl;
    }

    for(int i=1; i<4; i++){
        out<<"\n\n"<<indexName[i]<<endl;
        out<<" ,Before deletion, ,After deletion"<<endl;
        out<<"Load factor,Avg search time,Avg number of probes,";
        out<<"Avg search time,Avg number of probes"<<endl;
        for(int j=0; j<6; j++){
            out<<0.4 + 0.1*j;
            out<<","<<searchtime_predel[j][i]<<","<<probecount_predel[j][i];
            out<<","<<searchtime_postdel[j][i]<<","<<probecount_postdel[j][i]<<endl;
        }
    }

    for(int i=0; i<6; i++){
        out<<"\n\nLoad Factor = "<<0.4 + 0.1*i<<endl;
        out<<",Before Deletion, ,After Deletion,"<<endl;
        out<<"Method,Avg search time,Avg number of probes,";
        out<<"Avg search time,Avg number of probes"<<endl;
        for(int j=0; j<4; j++){
            out<<indexName[j]<<",";
            out<<searchtime_predel[i][j]<<",";
            (j==0)? out<<"N/A" : out<<probecount_predel[i][j];
            out<<","<<searchtime_postdel[i][j]<<",";
            (j==0)? out<<"N/A" : out<<probecount_postdel[i][j];
            out<<endl;
        }
    }

    out.close();
}