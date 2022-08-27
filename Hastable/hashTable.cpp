#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include "generator.cpp"
#define ll long long
#define ull unsigned long long
#define endl "\n";

struct hashNode{
    char key[8];
    int element;
    hashNode* next;

    hashNode(const char* k = "", const int e = -1, hashNode* n = nullptr){
        strcpy(key, k);
        element = e;
        next = n;
    }
};

class hashTable{
    int n;
    int N;
    int probeCount = 0;
    hashNode* DEL;
    hashNode* container;

    hashNode* (hashTable::*place_f)(const char*);
    hashNode* (hashTable::*get_f)(const char*);
    void (hashTable::*remove_f)(const char*);
    int (hashTable::*probe_f)(int);

    ll h1(const char* x){
        ll a = 33;
        return x[6] + a*(x[5] + a*(x[4] + a*(x[3] + a*(x[2] + a*(x[1] + a*x[0])))));
    }

    ll h2(const char* x){
        // ull result = 0;
        // for(int i=0; i<7; i++) result = result | ((x[i] - 'a') << i*sizeof(char));
        // return result % N;
        int a[] = {7,11,13,17,19,23,29};
        ll result = 0;
        for(int i=0; i<7; i++) result += a[i] * x[i]; 
        return result % N;
    }


    int linear_probe(int i){return i;}
    int quadratic_probe(int i){return (3*i + 5*i*i);}

    hashNode* place_seperate(const char* key){
        int hash = h1(key) % N;
        hashNode* curr = &container[hash];
        while(curr->next != nullptr){
            curr = curr->next;
            if(!strcmp(curr->key, key)) break;
        }
        if(!strcmp(curr->key, key)) return nullptr;
        return curr;
    }

    hashNode* place_open(const char* key){
        ll hash1 = h1(key);
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + (this->*probe_f)(i)) % N;
            hashNode* currNode = container[index].next;
            if(currNode == nullptr || currNode == DEL) break;
            if(!strcmp(key, currNode->key)) return nullptr;
        }
        if(container[index].next == nullptr || container[index].next == DEL)
            return &container[index];
        else return nullptr;
    }

    hashNode* place_double(const char* key){
        ll hash1 = h1(key), hash2 = h2(key);
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + i*hash2) % N;
            hashNode* currNode = container[index].next;
            if(currNode == nullptr || currNode == DEL) break;
            if(!strcmp(key, currNode->key)) return nullptr;
        }
        if(container[index].next == nullptr || container[index].next == DEL)
            return &container[index];
        else return nullptr;
    }

    hashNode* get_seperate(const char* key){
        int hash = h1(key) % N;
        hashNode* curr = container[hash].next;
        while(curr != nullptr){
            if(!strcmp(key, curr->key)) return curr;
            curr = curr->next;
        }
        return curr;
    }

    hashNode* get_open(const char* key){
        ll hash1 = h1(key);
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + (this->*probe_f)(i)) % N;
            hashNode* currNode = container[index].next;
            probeCount++;
            if(currNode == nullptr) break; 
            if(!strcmp(key, currNode->key)) return currNode;
        }
        return nullptr;
    }

    hashNode* get_double(const char* key){
        ll hash1 = h1(key), hash2 = h2(key);
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + i*hash2) % N;
            hashNode* currNode = container[index].next;
            probeCount++;
            if(currNode == nullptr) break; 
            if(!strcmp(key, currNode->key)) return currNode;
        }
        return nullptr;
    }

    void remove_seperate(const char* key){
        int hash = h1(key) % N;
        hashNode* prev = &container[hash];
        hashNode* curr = container[hash].next;
        while(curr != nullptr){
            if(!strcmp(key, curr->key)){
                prev->next = curr->next;
                delete curr;
                return;
            }
            curr = curr->next;
            prev = prev->next;
        }
    }

    void remove_open(const char* key){
        ll hash1 = h1(key) % N;
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + (this->*probe_f)(i)) % N;
            hashNode* currNode = container[index].next;
            if(currNode == nullptr) break;
            if(!strcmp(key, currNode->key)){
                container[index].next = nullptr;
                delete currNode;
                return;
            }
        }
    }

    void remove_double(const char* key){
        ll hash1 = h1(key), hash2 = h2(key);
        int index;
        for(int i=0; i<N; i++){
            index = (hash1 + i*hash2) % N;
            hashNode* currNode = container[index].next;
            if(currNode == nullptr) break;
            if(!strcmp(key, currNode->key)){
                container[index].next = nullptr;
                delete currNode;
                return;
            }
        }
    }

public:
    hashTable(int N, int type){
        //type = 1 : seperate chaining
        //type = 2 : linear probing
        //type = 3 : quadratic probing
        //type = 4 : double hashing

        this->N = N;
        DEL = new hashNode("DEL");
        container = new hashNode[N];
        n = 0;

        switch (type)
        {
        case 1:
            place_f = place_seperate;
            get_f = get_seperate;
            remove_f = remove_seperate;
            break;

        case 2:
            place_f = place_open;
            get_f = get_open;
            remove_f = remove_open;
            probe_f = linear_probe;
            break;
        
        case 3:
            place_f = place_open;
            get_f = get_open;
            remove_f = remove_open;
            probe_f = quadratic_probe;
            break;

        case 4:
            place_f = place_double;
            get_f = get_double;
            remove_f = remove_double;
            break;
        
        default:
            place_f = place_open;
            get_f = get_open;
            remove_f = remove_open;
            probe_f = linear_probe;
            break;
        }
    }

    ~hashTable(){
        clear();
        delete[] container;
        delete DEL;
    }

    void clear(){
        for(int i=0; i<N; i++){
            hashNode* curr = container[i].next;
            while(curr != nullptr){
                hashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
            container[i].next = nullptr;
        }
        n = 0;
    }

    void insert(const char* key, const int element){
        if(n == N) return;
        hashNode* place = (this->*place_f)(key);
        if(place == nullptr) return;
        hashNode* newNode = new hashNode(key, element);
        place->next = newNode;
        n++;
    }

    int get(const char* key){
        hashNode* result = (this->*get_f)(key);
        if(result == nullptr) return 0;
        return result->element;
    }

    void remove(const char* key){
        (this->*remove_f)(key);
        n--;
    }

    double load_factor(){return 1.00*n/N;}

    int h1_test(){
        std::vector<ll> hashList;
        char wordList[100][8] = {0};
        word_generator(wordList, 100);

        for(int i=0; i<100; i++){
            hashList.push_back(h1(wordList[i]) % N);
            // std::cout<<h1(wordList[i]) % N<<endl;
        }

        std::sort(hashList.begin(), hashList.end());
        int counter = 1;
        for(int i=1; i<hashList.size(); i++){
            if(hashList[i] != hashList[i-1]) counter++;
        }
        return counter;
    }

    int h2_test(){
        std::vector<ll> hashList;
        char wordList[100][8] = {0};
        word_generator(wordList, 100);

        for(int i=0; i<100; i++){
            hashList.push_back(h2(wordList[i]) % N);
        }

        std::sort(hashList.begin(), hashList.end());
        int counter = 1;
        for(int i=1; i<hashList.size(); i++){
            if(hashList[i] != hashList[i-1]) counter++;
        }
        return counter;
    }

    int get_probe_count(){return probeCount;}
    void set_probe_count(int p){probeCount = p;}
    int size(){return n;}
};

#endif
// int main(){
//     hashTable h(5, 4);
//     h.insert("ancient", 1);
//     h.insert("pazzled", 2);
//     h.insert("benefit", 3);
//     h.insert("lololol", 4);
//     h.insert("zigzags", 5);

//     std::cout<<h.get("ancient")<<endl;
//     std::cout<<h.get("pazzled")<<endl;
//     std::cout<<h.get("benefit")<<endl;
//     std::cout<<h.get("lololol")<<endl;
//     std::cout<<h.get("zigzags")<<endl;
//     std::cout<<endl;

//     h.remove("zigzags");
//     h.insert("zigzags", 100);
//     std::cout<<h.get("zigzags")<<endl;

//     h.clear();

//     h.insert("ancient", 1);
//     h.insert("pazzled", 2);
//     h.insert("benefit", 3);
//     h.insert("ancient", 4);
//     h.insert("zigzags", 5);

//     std::cout<<h.get("ancient")<<endl;
//     std::cout<<h.get("pazzled")<<endl;
//     std::cout<<h.get("benefit")<<endl;
//     std::cout<<h.get("ancient")<<endl;
//     std::cout<<h.get("zigzags")<<endl;
//     std::cout<<endl;

//     h.remove("zigzags");
//     h.insert("zigzags", 100);
//     std::cout<<h.get("zigzags")<<endl;
// }


// int main(){
//     hashTable h(109, 4);
//     std::cout<<h.h1_test()<<endl;
//     std::cout<<h.h2_test()<<endl;
// }