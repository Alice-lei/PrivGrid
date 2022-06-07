#ifndef __SHARE__
#define __SHARE__
#include "global.h"

void shareGen();
//定义CSV读类
class CSVreader
{
public:
    CSVreader(const char *);
    int readline();
    float data[58];

private:
    ifstream _csvInput;
};
#endif