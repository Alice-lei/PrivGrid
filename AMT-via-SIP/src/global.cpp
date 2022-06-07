#include "global.h"

bool role;
string IP = "127.0.0.1";
int PORT = 26481;
int dataSetNum = 5;
int dataSetStartIndex = 0;
int userStartIndex = 0;
int baseNum = 10;
int tripesFLAG = 0;
int shareStart = 0;
int shareEnd = 5;
int steps = 20;
int userCountsFLAG = 0;
int userLimit = 0;
mp_bitcnt_t randBit = 512;
mp_bitcnt_t eAndC = 30;

char tDelim[2] = "-";
char mDelim[2] = ",";
char iDelim[2] = ":";
char xDelim[2] = "|";
char checkMSG[3] = "ok";
char symbol[4] = {'|', '/', '-', '\\'};
string prefix[2] = {"Training_", "Test_"};

mpz_class modNum160{"1248878241464690322159961483223834105349154894079", 10}; //160bit
mpz_class modNum80{"618970019642690137449562111", 10};                        //80bit
mpz_class modNum[2] = {modNum160, modNum80};
int modNumIndex = 0;
//mpz_class eAndC{"4294967296", 10};//2^32
//mpz_class eAndC{"16777216", 10};//2^24
//string modNumStr = "40343";//小素数

// mpz_class sig0{"2147483648", 10};         //放大2^32倍, 0.5
// mpz_class sig1{"1071164843", 10};         //放大2^32倍, 0.2494
// mpz_class sig2{"0", 10};                  //放大2^32倍, 1.5491e-14
// mpz_class sig3{"-79027398", 10};          //放大2^32倍, -0.0184
// mpz_class learningRate{"1073741824", 10}; //放大2^32倍, 0.25
// mpz_class learningRate2{"107374182", 10}; //放大2^32倍, 0.025

mpz_class sig0{"536870912", 10};         //放大2^30倍, 0.5
mpz_class sig1{"267791211", 10};         //放大2^30倍, 0.2494
mpz_class sig2{"0", 10};                  //放大2^30倍, 1.5491e-14
mpz_class sig3{"-19756849", 10};          //放大2^30倍, -0.0184
mpz_class learningRate{"268435456", 10}; //放大2^30倍, 0.25
mpz_class learningRate2{"26843545", 10}; //放大2^30倍, 0.025

// mpz_class sig0{"524288", 10};         //放大2^20倍, 0.5
// mpz_class sig1{"261514", 10};         //放大2^20倍, 0.2494
// mpz_class sig2{"0", 10};                  //放大2^20倍, 1.5491e-14
// mpz_class sig3{"-19293", 10};          //放大2^20倍, -0.0184
// mpz_class learningRate{"262144", 10}; //放大2^20倍, 0.25
// mpz_class learningRate2{"26214", 10}; //放大2^20倍, 0.025

int m1m1m1_counts = 49626;
int m40m58m1_counts = 80;
int m40m40m1_counts = 472;
int m20m40m1_counts = 1;
int m1m20m1_counts = 1;
int m1m1m20_counts = 1;
int m20m20m1_counts = 1;
int m20m1m1_counts = 1;
int m20m1m40_counts = 1;
int m40m40m20_counts = 1;
int m40m20m1_counts = 1;
int m40m1m40_counts = 240;
int m40m1m58_counts = 80;

//int m1m1m1_counts = 1000;
//int m40m58m1_counts = 0;
//int m40m40m1_counts = 0;
//int m20m40m1_counts = 0;
//int m1m20m1_counts = 0;
//int m1m1m20_counts = 0;
//int m20m20m1_counts = 0;
//int m20m1m1_counts = 0;
//int m20m1m40_counts = 0;
//int m40m40m20_counts = 0;
//int m40m20m1_counts = 0;
//int m40m1m40_counts = 0;
//int m40m1m58_counts = 0;

int m1m1m1_num = 0;
int mTriples_num[12] = {0};
int triples_num[13] = {m1m1m1_counts,
                       m40m58m1_counts,
                       m40m40m1_counts,
                       m20m40m1_counts,
                       m1m20m1_counts,
                       m1m1m20_counts,
                       m20m20m1_counts,
                       m20m1m1_counts,
                       m20m1m40_counts,
                       m40m40m20_counts,
                       m40m20m1_counts,
                       m40m1m40_counts,
                       m40m1m58_counts};
time_t start, lastPoint;
double NetDuration = 0;
double speedS = 0, speedR = 0;
int countS = 0, countR = 0;

//将string转换为char*
char *stringToChar(string in_string)
{
    char *cstr = new char[in_string.size() + 1];
    strcpy(cstr, in_string.c_str());
    return cstr;
}

//在大素数范围内生成随机数
mpz_class randNumGen()
{
    random_device rd;
    default_random_engine e(rd());
    mpz_class ans, randNum;
    gmp_randstate_t grt;
    gmp_randinit_mt(grt);
    gmp_randseed_ui(grt, e());
    mpz_urandomb(randNum.get_mpz_t(), grt, randBit);
    mpz_mod(ans.get_mpz_t(), randNum.get_mpz_t(), modNum[modNumIndex].get_mpz_t());
    gmp_randclear(grt);
    return ans;
}

//在大素数范围内生成随机数
// mpz_class randNumGen(default_random_engine e,gmp_randstate_t grt)
// {
//     mpz_class ans, randNum;
//     mpz_urandomb(randNum.get_mpz_t(), grt, randBit);
//     mpz_mod(ans.get_mpz_t(), randNum.get_mpz_t(), modNum[modNumIndex].get_mpz_t());
//     return ans;
// }

void showTime()
{
    time_t point = time(nullptr);
    time_t allTime = (double)(point - start);
    time_t thisTime = (double)(point - lastPoint);
    cout << "Using time: " << thisTime / 3600 % 24 << "h " << thisTime / 60 % 60 << "m " << thisTime % 60 << "s | "
         << "Total time: " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m " << allTime % 60 << "s" << endl;
    if (SPEED)
    {
        cout << "Send speed: " << speedS / countS << "MiB/s | Recv speed:" << speedR / countR << "MiB/s"
             << endl;
        speedS = speedR = countS = countR = 0;
    }
    lastPoint = point;
}

int showTime(string word)
{
    cout << word << ": ";
    time_t point = time(nullptr);
    time_t allTime = (double)(point - start);
    time_t thisTime = (double)(point - lastPoint);
    cout << "Using time: " << thisTime / 3600 % 24 << "h " << thisTime / 60 % 60 << "m " << thisTime % 60 << "s | "
         << "Total time: " << allTime / 3600 / 24 << "d " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m " << allTime % 60 << "s" << endl;
    lastPoint = point;
    return thisTime;
}

void showTime(int flag)
{
    time_t point = time(nullptr);
    time_t allTime = (double)(point - start);
    cout << "Total time: " << allTime / 3600 / 24 << "d " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m " << allTime % 60 << "s" << endl;
    if (SPEED)
        cout << "Send speed: " << speedS << "MiB/s | Recv speed:" << speedR << "MiB/s"
             << endl;
}

int showNetTime(string word)
{
    int duration = (int)NetDuration;
    cout << word << ": " << duration << "ms" << endl;
    NetDuration = 0;
    return duration;
}

void mpz_print(mpz_class num, string out_word)
{
    cout << out_word << ": ";
    cout << num.get_str() << endl;
}

void dir()
{
    if (access("./output/", 0))
        mkdir("./output/", S_IRWXU | S_IRWXG | S_IRWXO);
}