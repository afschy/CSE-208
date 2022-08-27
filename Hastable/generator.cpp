#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <algorithm>
#include <ctime>

void word_generator(char** res, int n){
    srand(time(NULL));
    for(int i=0; i<n; i++){
        for(int j=0; j<7; j++){
            res [i][j] = 'a' + rand() % 26;
        }
        res[i][7] = 0;
    }
}

void word_generator(char res[][8], int n){
    srand(time(NULL));
    for(int i=0; i<n; i++){
        for(int j=0; j<7; j++){
            res [i][j] = 'a' + rand() % 26;
        }
        res[i][7] = 0;
    }
}

#endif