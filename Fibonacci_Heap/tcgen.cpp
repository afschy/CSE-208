#include <iostream>
#include <string>
#include <fstream>

int main(){
    std::ofstream out1, out2;
    out1.open("in5.txt");
    out2.open("in6.txt");
    int n;
    std::cin>>n;
    int m = n*(n-1)/2;
    out1<<n<<" "<<m<<"\n";
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            out1<<i<<" "<<j<<" "<<rand()%999 + 1<<"\n";
        }
    }
    out2<<"2\n";
    out2<<"0 "<<n-1<<"\n";
    out2<<"0 "<<n/2<<"\n";
    out1.close();
    out2.close();
    return 0;
}