#pragma once

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include "typechange.h"

class zipwrite {
public:
    std::ifstream infile;
    //FILE* outfile;
    UINT32 writeposition;
    unsigned char readdata;
    unsigned int headreaddata;
    unsigned int time;
    unsigned short filenameleng;
    unsigned int compressleng;

    zipwrite(unsigned int times);
    bool searchfile(char fn[], char* cdfn, unsigned short cdfnlen);
    void LoclheadAndDatacopy(unsigned long long position, FILE* wf, char* rfn);
};
