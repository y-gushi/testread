#pragma once

#include <stdio.h>

struct tnode {
    int wei = 0;
    int chi = 0;
    int num = 0;
    struct tnode* left = nullptr;
    struct tnode* right = nullptr;
};

struct tab {
    long wei = 0;
    int chi = 0;
    int num = 0;
    struct tab* next = nullptr;
};


