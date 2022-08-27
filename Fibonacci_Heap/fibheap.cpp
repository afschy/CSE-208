#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <queue>
#include <ctime>
#include <chrono>

#define ll long long
#define endl "\n"
#define UNDEFINED -1
#define INF 9223372036854775807

template<typename elemType, typename keyType>
struct node{
    elemType element;
    keyType key;
    
    node<elemType,keyType>* parent;
    node<elemType,keyType>* child;
    node<elemType,keyType>* left;
    node<elemType,keyType>* right;

    int degree;
    bool mark;

    node(const elemType& e = UNDEFINED, const keyType& k = UNDEFINED){
        element = e;
        key = k;
    }
};

template<typename elemType, typename keyType>
class fibHeap{
    int n;
    node<elemType,keyType>* min;

    void insert_to_row(node<elemType,keyType>* newNode){
        (min->left)->right = newNode;
        newNode->left = min->left;
        newNode->right = min;
        min->left = newNode;
    }

    void consolidate(){
        int D = 1.45*std::log2(1.00*n) + 1;
        node<elemType,keyType>** A = new node<elemType,keyType>*[D+1];
        for (int i=0; i<=D; i++)
            A[i] = nullptr;

        std::vector<node<elemType,keyType>*> nodeList;
        node<elemType,keyType>* x = min;
        do{
            nodeList.push_back(x);
            // std::cout<<"a\n";
            x = x->right;
        } while(x != min);

        for(int i=0; i<nodeList.size(); i++){
            x = nodeList[i];
            if(x->parent != nullptr) continue;
            int d = x->degree; 

            while (A[d] != nullptr) { 
                node<elemType,keyType>* y = A[d];
                if (x->key > y->key) { 
                    node<elemType,keyType>* temp = x; 
                    x = y; 
                    y = temp; 
                } 
                link(x, y); 
                A[d] = NULL; 
                d++; 
            }

            A[d] = x; 
        }

        min = nullptr;
        for(int i=0; i<=D; i++){
            if(A[i] == nullptr) continue;
            A[i]->left = A[i]->right = A[i];
            if(min == nullptr){
                min = A[i];
            }
            else{
                insert_to_row(A[i]);
                if(A[i]->key < min->key) min = A[i];
            }
        }

        delete[] A;
    }

    void link(node<elemType,keyType>* x, node<elemType,keyType>* y){
        (y->left)->right = y->right;
        (y->right)->left = y->left;
        y->left = y->right = y;
        y->parent = x;
        y->mark = false;

        
        if(x->child == nullptr) x->child = y;
        else{
            node<elemType,keyType>* child = x->child;
            y->right = child;
            y->left = child->left;
            (child->left)->right = y;
            child->left = y;
            if(y->key < child->key)
                x->child = y;
        }

        x->degree++;
    }

    void cut(node<elemType,keyType>* x, node<elemType,keyType>* y){
        if(x == x->right) y->child = nullptr;
        else if(x == y->child) y->child = x->right;

        (x->left)->right = x->right;
        (x->right)->left = x->left;
        y->degree--;

        insert_to_row(x);
        x->parent = nullptr;
        x->mark = false;
    }

    void cascading_cut(node<elemType,keyType>* y){
        node<elemType,keyType>* z = y->parent;
        if(z==nullptr) return;

        if(y->mark == false) y->mark = true;
        else{
            cut(y, z);
            cascading_cut(z);
        }
    }

public:
    fibHeap(){
        n = 0;
        min = nullptr;
    }

    void insert(node<elemType,keyType>* newNode){
        newNode->degree = 0;
        newNode->parent = nullptr;
        newNode->child = nullptr;
        newNode->left = newNode;
        newNode->right = newNode;
        newNode->mark = false;

        if(min != nullptr){
            insert_to_row(newNode);
            if(newNode->key < min->key) min = newNode;
        }
        else min = newNode;
        n++;
    }

    void insert(int element, ll key){
        node<elemType,keyType>* newNode = new node<elemType,keyType>(element, key);
        insert(newNode);
    }

    void merge(fibHeap& ob){
        node<elemType,keyType>* min2 = ob.min;
        if(min2 == nullptr) return;
        else if(min == nullptr){
            min = min2;
            return;
        }

        if(min2->key < min->key){
            node<elemType,keyType>* temp = min;
            min = min2;
            min2 = temp;
        }

        node<elemType,keyType>* min_right = min->right;
        node<elemType,keyType>* min2_left = min2->left;
        min->right = min2;
        min2->left = min;
        min_right->left = min2_left;
        min2_left->right = min_right;

        n += ob.n;
        ob.min = nullptr;
        ob.n = 0;
    }

    node<elemType,keyType>* extract_min(){
        node<elemType,keyType>* z = min;
        if(z==nullptr) return nullptr;

        if(z->child != nullptr){
            node<elemType,keyType>* currChild = z->child;
            while(true){
                node<elemType,keyType>* nextChild = currChild->right;
                currChild->parent = nullptr;
                insert_to_row(currChild);
                if(nextChild == z->child) break;
                currChild = nextChild;
            }

            z->child = nullptr;
        }

        (z->right)->left = z->left;
        (z->left)->right = z->right;
        if(z->right == z) min = nullptr;
        else{
            min = z->right;
            consolidate();
        }
        n--;

        return z;
    }

    void decrease_key(node<elemType,keyType>* x, ll k){
        if(k > x->key) return;
        x->key = k;
        node<elemType,keyType>* y = x->parent;
        if(y!=nullptr && x->key < y->key){
            cut(x, y);
            cascading_cut(y);
        }
        if(x->key < min->key) min = x;
    }

    void remove(node<elemType,keyType>* x){
        decrease_key(x, -INF);
        extract_min();
    }

    bool empty(){return n<=0;}
    node<elemType,keyType>* get_min_node(){return min;}


    void display(){
        if(min==nullptr){
            std::cout<<"empty\n";
            return;
        }
        node<elemType,keyType>* curr = min;
        do{
            std::cout<<curr->element<<" ";
            curr =  curr->right;
        } while(curr != min);
        std::cout<<endl;
    }
};