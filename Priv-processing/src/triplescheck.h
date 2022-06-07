#ifndef __CHECK__
#define __CHECK__

#include "mulgate.h"

int check()
{
    MatrixTools ckTool;
    Triples ckTriples;
    bool flag = false;
    string sName, cName;
    mpz_class index1, index2;
    int type;
    cout << "Input SERVER file path:" << flush;
    cin >> sName;
    cout << "Input CLIENT file path:" << flush;
    cin >> cName;
    cout << "Type: (1)int (2)matrix :" << flush;
    cin >> type;
    start = time(nullptr);
    if (type == 1)
    {
        mpz_class a0, b0, c0, a1, b1, c1;
        mpz_class A, B, C, temp_mul, ans;
        ifstream infileS, infileC;
        string lineS, lineC;
        infileS.open(sName, ios::in);
        infileC.open(cName, ios::in);
        if (!infileS)
            cout << "Read SERVER's file failed!" << endl;
        if (!infileC)
            cout << "Read CLIENT's file failed!" << endl;
        while ((getline(infileS, lineS) && infileS.good() && !infileS.eof() && !lineS.empty()) &&
               (getline(infileC, lineC) && infileC.good() && !infileC.eof() && !lineC.empty()))
        {
            ckTriples.deserialization(lineS, index1, a0, b0, c0);
            ckTriples.deserialization(lineC, index2, a1, b1, c1);
            ckTool.mojia(a0, a1, A);
            ckTool.mojia(b0, b1, B);
            ckTool.mojia(c0, c1, C);
            ckTool.mLocalMocheng(A, B, temp_mul);
            mpz_sub(ans.get_mpz_t(), C.get_mpz_t(), temp_mul.get_mpz_t());
            if (ans == 0)
                flag = true;
        }
        infileS.close();
        infileC.close();
    }
    else if (type == 2)
    {
        Matrix a0, b0, c0, a1, b1, c1;
        Matrix A, B, C, temp_mul, ans;
        ifstream infileS, infileC;
        string lineS, lineC;
        infileS.open(sName, ios::in);
        infileC.open(cName, ios::in);
        if (!infileS)
            cout << "Read SERVER's file failed!" << endl;
        if (!infileC)
            cout << "Read CLIENT's file failed!" << endl;
        while ((getline(infileS, lineS) && infileS.good() && !infileS.eof() && !lineS.empty()) &&
               (getline(infileC, lineC) && infileC.good() && !infileC.eof() && !lineC.empty()))
        {
            ckTriples.deserialization(lineS, index1, a0, b0, c0);
            ckTriples.deserialization(lineC, index2, a1, b1, c1);
            ckTool.mAdd(a0, a1, A);
            ckTool.mAdd(b0, b1, B);
            ckTool.mAdd(c0, c1, C);
            ckTool.mLocalMul(A, B, temp_mul);
            ckTool.mSub(C, temp_mul, ans);
            Matrix const_zero{M_NORMAL, 0, ans.row, ans.col};
            if (ckTool.mCmp(ans, const_zero))
                flag = true;
        }
        infileS.close();
        infileC.close();
    }
    else
    {
        printf("ERROR\n");
        exit(1);
    }
    if (flag)
        cout << "Triples PASS" << endl;
    else
        cout << "Triples WRONG" << endl;
    return 0;
}

#endif
