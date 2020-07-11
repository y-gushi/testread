#pragma once

#include "typechange.h"
#include <time.h>
#include <stdio.h>
#include <iostream>

class LHmake {
public:
    UINT32 times = 0;
    UINT16 days = 0;
    time_t now;
    struct tm tn;

    LHmake();
    void getday();
};