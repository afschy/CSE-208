#include <iostream>
#include <algorithm>
#define UNDEFINED -1
#define endl "\n"

enum Color{RED, BLACK};

struct node{
    int key;
    node* left;
    node* right;
    node* parent;
    Color color;
    int desc_num;

    node(int k = -1){
        key = k;
        left = right = parent = nullptr;
        desc_num = 1;
    }
};

class rbtree{
    node* root;
    node* NIL;
    int n;

    void desc_update_single(node* x){x->desc_num = 1 + (x->left)->desc_num + (x->right)->desc_num;}
    void desc_update_chain(node* curr){
        if(curr == NIL) return;
        desc_update_single(curr);
        desc_update_chain(curr->parent);
    }

    void left_rotate(node* x){
        node* y = x->right;
        x->right = y->left;
        if(y->left != NIL)
            (y->left)->parent = x;
        y->parent = x->parent;
        if(x->parent == NIL) root = y;
        else if(x == (x->parent)->left) (x->parent)->left = y;
        else (x->parent)->right = y;
        y->left = x;
        x->parent = y;

        desc_update_single(x);
        desc_update_single(y);
    }

    void right_rotate(node* x){
        node* y = x->left;
        x->left = y->right;
        if(y->right != NIL)
            (y->right)->parent = x;
        y->parent = x->parent;
        if(x->parent == NIL) root = y;
        else if(x == (x->parent)->left) (x->parent)->left = y;
        else (x->parent)->right = y;
        y->right = x;
        x->parent = y;

        desc_update_single(x);
        desc_update_single(y);
    }

    void insert_fixup(node* z){
        while((z->parent)->color == RED){
            node* parent = z->parent;
            node* grandparent = parent->parent;

            if(parent == grandparent->left){
                node* uncle = grandparent->right;

                if(uncle->color == RED){
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                }

                else{
                    if(z == parent->right){
                        z = parent;
                        left_rotate(z);
                        parent = z->parent;
                        grandparent = parent->parent;
                    }

                    parent->color = BLACK;
                    grandparent->color = RED;
                    right_rotate(grandparent);
                }
            }

            else{
                node* uncle = grandparent->left;

                if(uncle->color == RED){
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                }

                else{
                    if(z== parent->left){
                        z = parent;
                        right_rotate(z);
                        parent = z->parent;
                        grandparent = parent->parent;
                    }

                    parent->color = BLACK;
                    grandparent->color = RED;
                    left_rotate(grandparent);
                }
            }
        }

        root->color = BLACK;
    }

    void transplant(node* oldNode, node* newNode){
        if(oldNode->parent == NIL) root = newNode;
        else if(oldNode == (oldNode->parent)->left) (oldNode->parent)->left = newNode;
        else (oldNode->parent)->right = newNode;
        newNode->parent = oldNode->parent;
    }

    void remove_fixup(node* x){
        while(x != root && x->color == BLACK){
            node* parent = x->parent;

            if(x == parent->left){
                node* sibling = parent->right;
                if(sibling->color == RED){
                    sibling->color = BLACK;
                    parent->color = RED;
                    left_rotate(parent);
                    parent = x->parent;
                    sibling = parent->right;
                }

                if((sibling->left)->color == BLACK && (sibling->right)->color == BLACK){
                    sibling->color = RED;
                    x = parent;
                }

                else{
                    if((sibling->right)->color == BLACK){
                        (sibling->left)->color = BLACK;
                        sibling->color = RED;
                        right_rotate(sibling);
                        parent = x->parent;
                        sibling = parent->right;
                    }

                    sibling->color = parent->color;
                    parent->color = BLACK;
                    (sibling->right)->color = BLACK;
                    left_rotate(parent);
                    x = root;
                }
            }

            else{
                node* sibling = parent->left;
                if(sibling->color == RED){
                    sibling->color = BLACK;
                    parent->color = RED;
                    right_rotate(parent);
                    parent = x->parent;
                    sibling = parent->left;
                }

                if((sibling->left)->color == BLACK && (sibling->right)->color == BLACK){
                    sibling->color = RED;
                    x = parent;
                }

                else{
                    if((sibling->left)->color == BLACK){
                        (sibling->right)->color = BLACK;
                        sibling->color = RED;
                        left_rotate(sibling);
                        parent = x->parent;
                        sibling = parent->left;
                    }

                    sibling->color = parent->color;
                    parent->color = BLACK;
                    (sibling->left)->color = BLACK;
                    right_rotate(parent);
                    x = root;
                }
            }
        }

        x->color = BLACK;
    }

    void unsafe_delete(node* x){
        if(x == NIL) return;
        unsafe_delete(x->left);
        unsafe_delete(x->right);
        delete x;
    }

    node* get_min_node(node* curr){
        while(curr->left != NIL)
            curr = curr->left;
        return curr;
    }

    node* search_key(int key, node* curr){
        if(curr == NIL) return NIL;
        else if(curr->key == key) return curr;
        else if(key < curr->key) return search_key(key, curr->left);
        else return search_key(key, curr->right);
    }

    void printHelp(const node* currNode){
        std::cout<<"(";
        if(currNode != NIL){
            std::cout<<" "<<currNode->key;
            if(currNode->color == RED) std::cout<<"r";
            else std::cout<<"b";
            if(currNode->left != NIL || currNode->right != NIL){
                printHelp(currNode->left);
                printHelp(currNode->right);
            }
            std::cout<<" ";
        }
        std::cout<<")";
    }

    int smaller_count_help(const node* currNode, const int key){
        if(currNode == NIL) return 0;  
        if(currNode->key < key)
            return 1 + (currNode->left)->desc_num + smaller_count_help(currNode->right, key);
        return smaller_count_help(currNode->left, key);
    }

public:
    rbtree(){
        NIL = new node();
        NIL->color = BLACK;
        NIL->desc_num = 0;
        root = NIL;
        n = 0;
    }
    ~rbtree(){
        unsafe_delete(root);
        delete NIL;
    }

    int get_min(){
        if(root == NIL) return UNDEFINED;
        return get_min_node(root)->key;
    }

    bool search(int key){
        node* x = search_key(key, root);
        if(x == NIL) return false;
        return true;
    }

    void insert(int key){
        node* newNode = new node(key);
        insert(newNode);
    }

    void insert(node* newNode){
        node* y = NIL;
        node* x = root;
        while(x != NIL){
            y = x;
            if(newNode->key < x->key) x = x->left;
            else x = x->right;
        }

        newNode->parent = y;
        if(y == NIL) root = newNode;
        else if(newNode->key < y->key) y->left = newNode;
        else y->right = newNode;

        desc_update_chain(newNode->parent);

        newNode->left = newNode->right = NIL;
        newNode->color = RED;
        insert_fixup(newNode);
        n++;
    }

    void remove(int key){
        node* x = search_key(key, root);
        if(x == NIL) return;
        remove(x);
    }

    void remove(node* z){
        node *x, *y = z;
        Color y_original = y->color;
        
        if(z->left == NIL){
            x = z->right;
            transplant(z, x);
        }

        else if(z->right == NIL){
            x = z->left;
            transplant(z, x);
        }

        else{
            y = get_min_node(z->right);
            y_original = y->color;
            x = y->right;

            if(y->parent == z) x->parent = y;
            else{
                transplant(y, y->right);
                y->right = z->right;
                (y->right)->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            (y->left)->parent = y;
            y->color = z->color;
        }

        desc_update_chain(x->parent);
        if(y_original == BLACK)
            remove_fixup(x);
        n--;
        delete z;
    }

    int size(){return n;}

    int smaller_count(int key){
        return smaller_count_help(root, key);
    }

    void print(){
        if(root == NIL){
            std::cout<<"()\n";
            return;
        }
        std::cout<<root->key;
        if(root->color == RED) std::cout<<"r";
        else std::cout<<"b";
        printHelp(root->left);
        printHelp(root->right);
        std::cout<<"\n";
    }
};