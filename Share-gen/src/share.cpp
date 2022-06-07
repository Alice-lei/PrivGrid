#include "share.h"

void shareGen(string cwd, bool compressFlag)
{
    start = time(nullptr);
    //寻找数据文件
    DIR *dir = opendir(cwd.c_str());
    vector<string> filenames;
    vector<string> sourcePath;
    vector<string> serverPath;
    vector<string> clientPath;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && hasEnding(entry->d_name, ".csv"))
        {
            filenames.push_back(entry->d_name);
        }
    }
    closedir(dir);
    if (filenames.size() == 0)
    {
        cout << "No data files found" << endl;
        return;
    }
    sort(filenames.begin(), filenames.end());

    int index = 0;
    for (auto filename : filenames)
    {
        sourcePath.push_back(cwd + "/" + filename);
        serverPath.push_back("./output/server/SERVER_" + to_string(index) + ".dat");
        clientPath.push_back("./output/client/CLIENT_" + to_string(index) + ".dat");
        ++index;
    }

    cout << "Find " << sourcePath.size() << " files:" << endl;
    for (auto iter : sourcePath)
    {
        cout << iter << endl;
    }

    if (shareEnd == 0)
    {
        shareEnd = filenames.size();
    }
    cout << endl;
    // 处理数据
    for (int k = shareStart; k < shareEnd; k++)
    {
        cout << sourcePath[k] << endl;
        ifstream _csvInput;
        _csvInput.open(sourcePath[k], ios::in);
        //定义一行数据为字符串
        string _Oneline;
        ofstream outfile1, outfile2;
        outfile1.open(serverPath[k], ios::out | ios::trunc);
        outfile2.open(clientPath[k], ios::out | ios::trunc);
        mpf_class x_f;
        mpz_class x_z;
        mpz_class server, client;
        vector<array<string, 480>> valueLists;
        const char *delim = ",";
        cout << "Load file" << flush;
        while (getline(_csvInput, _Oneline))
        {
            array<string, 480> valueTemp;
            char *buffer = new char[_Oneline.size() + 1];
            buffer[_Oneline.size()] = '\0';
            copy(_Oneline.begin(), _Oneline.end(), buffer);
            char *p = std::strtok(buffer, delim);
            valueTemp[0] = p;
            for (int r = 1; r < 480; r++)
            {
                p = std::strtok(NULL, delim);
                valueTemp[r] = p;
            }
            valueLists.push_back(valueTemp);
            delete (buffer);
        }
        _csvInput.close();
        int userCounts = valueLists.size();
        stringstream ciphertextServer, ciphertextClient;
        stringstream ssServerStr, ssClientStr;
        random_device rd;
        default_random_engine e(rd());
        for (int p = 0; p < userCounts; p++)
        {
            gmp_randstate_t grt;
            gmp_randinit_mt(grt);
            gmp_randseed_ui(grt, e());
            array<string, 480> userValue = valueLists[p];
            //一个人有480个数据点
            for (int r = 0; r < 480; r++)
            {
                x_f = stof(userValue[r]); //数据点
                mpf_mul_2exp(x_f.get_mpf_t(), x_f.get_mpf_t(), eAndC);
                server = randNumGen(e, grt);
                x_z = x_f;
                client = x_z + modNum[modNumIndex] - server;
                ssServerStr << server.get_str();
                ssClientStr << client.get_str();
                int time = r % 48;
                int day = (r + 4) % 7;
                //将一行数据按'，'分割
                for (int i = 0; i < 57; i++)
                {
                    if ((i < 48 && i == time) || (i < 55 && i - 48 == day))
                        x_f = 1;
                    else
                        x_f = 0;
                    mpf_mul_2exp(x_f.get_mpf_t(), x_f.get_mpf_t(), eAndC);
                    server = randNumGen(e, grt);
                    x_z = x_f;
                    client = x_z + modNum[modNumIndex] - server;
                    ssServerStr << "," << server.get_str();
                    ssClientStr << "," << client.get_str();
                }
                ssServerStr << "\n";
                ssClientStr << "\n";
            }
            ciphertextServer << ssServerStr.str();
            ciphertextClient << ssClientStr.str();
            ssServerStr.str("");
            ssClientStr.str("");
            cout << "\rEncrypt " << std::fixed << std::setprecision(1) << p / (float)userCounts * 100 << "%" << flush;
            gmp_randclear(grt);
        }
        cout << "\rFile writing...\r" << flush;
        outfile1 << ciphertextServer.str();
        outfile2 << ciphertextClient.str();
        outfile1.close();
        outfile2.close();
        cout << "\rProcess Completed" << endl;
    }
    cout << "Share Gen Completed" << endl;
}