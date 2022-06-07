#ifndef __GLOBAL__
#define __GLOBAL__

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <array>
#include <cassert>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;

//#define eRole bool
#define SERVER true
#define CLIENT false
#define M_DIAGONAL 0
#define M_NORMAL 1
#define M_CIPHER 2
#define M_GAUSS true
#define RELEASE true
#define TRAIN true
#define TEST false
//#define RELEASE false
//#define RENAME true
#define RENAME false
//#define SPEED true
#define SPEED false
extern bool role;
extern string IP;
extern int PORT;
extern int dataSetNum;
extern int dataSetStartIndex;
extern int userStartIndex;
extern int baseNum;
extern int tripesFLAG;
extern int shareStart;
extern int shareEnd;
extern int steps;
extern int userCountsFLAG;
extern int userLimit;
extern int modNumIndex;
extern mpz_class modNum[2];
extern mp_bitcnt_t eAndC;
extern mp_bitcnt_t randBit;
extern char symbol[4];
extern char tDelim[2];
extern char mDelim[2];
extern char iDelim[2];
extern char xDelim[2];
extern char checkMSG[3];
extern string prefix[2];
extern mpz_class sig0;
extern mpz_class sig1;
extern mpz_class sig2;
extern mpz_class sig3;
extern mpz_class learningRate;
extern mpz_class learningRate2;
extern int m1m1m1_counts;
extern int m40m58m1_counts;
extern int m40m40m1_counts;
extern int m20m40m1_counts;
extern int m1m20m1_counts;
extern int m1m1m20_counts;
extern int m20m20m1_counts;
extern int m20m1m1_counts;
extern int m20m1m40_counts;
extern int m40m40m20_counts;
extern int m40m20m1_counts;
extern int m40m1m40_counts;
extern int m40m1m58_counts;
extern int m1m1m1_num;
extern int mTriples_num[12];
extern int triples_num[13];
extern time_t start, lastPoint;
extern double NetDuration;
extern double speedS, speedR;
extern int countS, countR;

//将string转换为char*
char *stringToChar(string in_string);

//在大素数范围内生成随机数
mpz_class randNumGen();
//在大素数范围内生成大量随机数
inline mpz_class randNumGen(default_random_engine e, gmp_randstate_t grt)
{
    mpz_class ans, randNum;
    mpz_urandomb(randNum.get_mpz_t(), grt, randBit);
    mpz_mod(ans.get_mpz_t(), randNum.get_mpz_t(), modNum[modNumIndex].get_mpz_t());
    return ans;
}

inline bool hasEnding(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

void showTime();

int showTime(string word);

void showTime(int flag);

int showNetTime(string word);

void mpz_print(mpz_class num, string out_word);

void dir();

#endif
