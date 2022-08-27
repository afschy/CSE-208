#include "rbtree.cpp"

int main(){
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    int n, e, x, result;
    std::cin>>n;
    std::cout<<n<<endl;
    rbtree t;

    for(int i=0; i<n; i++){
        std::cin>>e>>x;
        switch(e){
            case 0:
                if(!t.search(x)) result = 0;
                else{
                    result = 1;
                    t.remove(x);
                }
                break;

            case 1:
                if(t.search(x)) result = 0;
                else{
                    result = 1;
                    t.insert(x);
                }
                break;

            case 2:
                if(t.search(x)) result = 1;
                else result = 0;
                break;

            case 3:
                result = t.smaller_count(x);
                break;

            default:
                std::cout<<"Invalid Command\n";
        }
        std::cout<<e<<" "<<x<<" "<<result<<endl;
    }
}