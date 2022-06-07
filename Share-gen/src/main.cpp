#include "main.h"

void printHelp();

int main(int argc, char *argv[])
{
    start = time(nullptr);
    lastPoint = start;
    char action = 'x', cRole = 'x';
    int epochsT = 1, epochsP = 1, ch;
    string cwd = getcwd(NULL, 0);
    bool compressFlag = false;
    if (argc == 1)
    {
        printHelp();
        return 0;
    }
    else
    {
        while ((ch = getopt(argc, argv, "s:ch")) != -1)
        {
            switch (ch)
            {
            case 's':
                cwd = optarg;
                break;
            case '?':
                printf("unknow option:%c\n", optopt);
            case 'h':
            default:
                printHelp();
                return 0;
            }
        }
    }
    cout << "Share Gen" << endl;
    dir(); //确认文件夹
    shareGen(cwd, compressFlag);
    showTime(1);
    return 0;
}

void printHelp()
{
    cout << "\n Usage :\n"
         << " Share-gen [Options] <Destination>\n\n"
         << " Options  Destination\n"
         << " -s       (Required) Path to the directory where the data files are stored (*.csv)\n"
         << " -h       Help\n"
         << endl;
}