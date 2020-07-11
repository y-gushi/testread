#pragma once
#include <xlocinfo>

struct compdata {
	unsigned char* data=(unsigned char*)malloc(sizeof(unsigned char)*50000);
	unsigned char* rootdata;
	unsigned long len=0;
};