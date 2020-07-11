#include "timeget.h"

LHmake::LHmake() {
    now = time(NULL);
    //tn = localtime(&now);
    localtime_s(&tn, &now);
}

void LHmake::getday()
{
    //‘‚«‚Ş‚½‚ß2byte‚É‡‚í‚¹‚é
    times = (tn.tm_year+1900)%100;
    //int a = times & 0x7F;
    //std::cout << "year:" << tn.tm_year << " time:" << a << std::endl;
    times = ((times & 0x7F) << 26);

    times += (((tn.tm_mon) & 0x0F) << 21);
    //int b = (tn.tm_mon) & 0x0F;
    //std::cout << "month:" << tn.tm_mon << " time:" << b << std::endl;
    times += ((tn.tm_mday & 0x1F) << 16);
    //b = (tn.tm_mday) & 0x1F;
    //std::cout << "day:" << tn.tm_mday << " time:" << b << std::endl;

    times += ((tn.tm_hour & 0x1F) << 11);
    //b = (tn.tm_hour) & 0x1F;
    //std::cout << "hour:" << tn.tm_hour << " time:" << b << std::endl;
    times += ((tn.tm_min & 0x1F) << 6);
    //b = (tn.tm_min) & 0x1F;
    //std::cout << "minits:" << tn.tm_min << " time:" << b << std::endl;
    times += ((tn.tm_sec) & 0x3F);
    //b = (tn.tm_sec) & 0x3F;
    std::cout << "seconds:" << tn.tm_sec << " time:" << times << std::endl;
}