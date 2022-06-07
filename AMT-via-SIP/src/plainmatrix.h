#ifndef LSTM_PLAINMATRIX_H
#define LSTM_PLAINMATRIX_H

#include "mulgate.h"

void plainMatrix()
{
    start=time(NULL);
    Network net;
    MatrixTools tool;
    TriplesMul tml;
    int end, flag = 0;
    cout << "input end:" << flush;
    cin >> end;
    string str[28];
    if (end == -1)
    {
        end = 0;
        flag = 1;
    }

    array<string,28> mName{"Wfx1","Wix1","Wgx1","Wox1","Wfh1","Wih1","Wgh1","Woh1","bf1","bi1","bg1","bo1","Wfx2","Wix2",
"Wgx2","Wox2","Wfh2","Wih2","Wgh2","Woh2","bf2","bi2","bg2","bo2","W3","b3","W4","b4"};

    for (int i = 0; i <= end; i++)
    {
        string fileNameServer, fileNameClient, fileName;
        cout << "[" << setiosflags(ios::fixed) << setprecision(2) << (i) * 100.0 / end << "%] "
             << symbol[i / 5 % 4] << " \r" << flush;
        if (flag)
        {

            fileNameServer = "./input/matrix/SERVER_save.dat";
            fileNameClient = "./input/matrix/CLIENT_save.dat";
            fileName = "./input/matrix/save.csv";
        } else
        {
            fileNameServer = "./input/matrix/round/SERVER/SERVER_round_" + to_string(i) + "_save.dat";
            fileNameClient = "./input/matrix/round/CLIENT/CLIENT_round_" + to_string(i) + "_save.dat";
            fileName = "./input/matrix/round/PLAIN/round_" + to_string(i) + "_save.csv";
        }
        ifstream infileServer, infileClient;
        infileServer.open(fileNameServer, ios::in);
        infileClient.open(fileNameClient, ios::in);
        FILE *outfile = fopen(fileName.c_str(), "w");
        Matrix mSer[28], mCli[28], m[28];
        if (infileServer)
        {
            for (int s = 0; s < 28; s++)
            {
                getline(infileServer, str[s]);
                net.deserialization(str[s], mSer[s]);
            }
        } else
            cout << "Load error!" << endl;
        if (infileClient)
        {
            for (int c = 0; c < 28; c++)
            {
                getline(infileClient, str[c]);
                net.deserialization(str[c], mCli[c]);
            }
        } else
            cout << "Load error!" << endl;
        infileServer.close();
        infileClient.close();

        for (int j = 0; j < 28; j++)
        {
            fprintf(outfile, "\n%s\n", mName[j].c_str());
            tool.mAdd(mSer[j], mCli[j], m[j]);
            for (int row = 0; row < m[j].row; row++)
            {
                for (int col = 0; col < m[j].col; col++)
                {
                    mpf_class tempF;
                    if (m[j].matrix[row][col].get_d() != 0)
                    {
                        mpf_class tempF1{m[j].matrix[row][col]};
                        if (tempF1 > modNum[modNumIndex] / 2)
                            tempF1 -= modNum[modNumIndex];
                        mpf_div_2exp(tempF.get_mpf_t(), tempF1.get_mpf_t(), eAndC);
                        gmp_fprintf(outfile, "%.5Ff,", tempF.get_mpf_t());
                    } else
                        gmp_fprintf(outfile, "%Zd,",  m[j].matrix[row][col].get_mpz_t());
                }
                fprintf(outfile, "\n");
            }

        }
        fclose(outfile);
    }
}

#endif //LSTM_PLAINMATRIX_H
