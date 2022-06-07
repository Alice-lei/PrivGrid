#include "main.h"

void printHelp();

int main(int argc, char *argv[])
{
    start = time(nullptr);
    lastPoint = start;
    char action = 'x', cRole = 'x';
    int epochsT = 1, epochsP = 1, ch;
    string dirData="./input/data/", dirTriples="./input/triples/";
    if (argc == 1)
    {
        printHelp();
        return 0;
    }
    else
    {
        while ((ch = getopt(argc, argv, "r:i:p:d:t:h")) != -1)
        {
            switch (ch)
            {
            case 'r':
                role = (!strcmp(optarg, "1")) ? SERVER : CLIENT;
                break;
            case 'i':
                IP = optarg;
                break;
            case 'p':
                PORT = stoi(optarg);
                break;
            case 'd':
                dirData = optarg;
                break;
            case 't':
                dirTriples = optarg;
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
    cout << "You are " << ((role == SERVER) ? "SERVER" : "CLIENT") << endl;

    dir(); //确认文件夹
    Lstm lstm;

    lstm.train(dirData,dirTriples);
    showTime(1);
    return 0;
}

void printHelp()
{
    cout << "\n Usage :\n"
         << " Priv-processing [Options] <Destination>\n\n"
         << " Options  Destination\n"
         << " -r       (required) Role, input integer 1 or 2. 1 means server, 2 means client.\n"
         << " -i       (optional) IP Address. Default 127.0.0.1\n"
         << " -p       (optional) Port Number. Default 26481\n"
         << " -h       Help\n"
         << endl;
}