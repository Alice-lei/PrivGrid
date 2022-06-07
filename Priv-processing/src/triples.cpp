#include "triples.h"

using namespace std;

//生成查找表
Triples::Triples()
{
    //查找表，用于找到对应尺寸的三元组
    for (int i = 0; i < 58; i++)
        for (int j = 0; j < 58; j++)
            for (int k = 0; k < 58; k++)
                this->table[i][j][k] = -1;
    table[40][58][1] = this->m40m58m1;
    table[40][40][1] = this->m40m40m1;
    table[20][40][1] = this->m20m40m1;
    table[1][20][1] = this->m1m20m1;
    table[1][1][20] = this->m1m1m20;
    table[20][20][1] = this->m20m20m1;
    table[20][1][1] = this->m20m1m1;
    table[20][1][40] = this->m20m1m40;
    table[40][40][20] = this->m40m40m20;
    table[40][20][1] = this->m40m20m1;
    table[40][1][40] = this->m40m1m40;
    table[40][1][58] = this->m40m1m58;
}

//读入文件中的三元组
void Triples::init(int flag, string prefixString, string dirTriples)
{
    cout << "Triples preparing " << flush;
    this->dirTriples = dirTriples;
    this->readIntTriples(flag, prefixString);
    this->readMatrixTriples(40, 58, 1, flag, prefixString);
    this->readMatrixTriples(40, 40, 1, flag, prefixString);
    this->readMatrixTriples(20, 40, 1, flag, prefixString);
    this->readMatrixTriples(1, 20, 1, flag, prefixString);
    this->readMatrixTriples(1, 1, 20, flag, prefixString);
    this->readMatrixTriples(20, 20, 1, flag, prefixString);
    this->readMatrixTriples(20, 1, 1, flag, prefixString);
    this->readMatrixTriples(20, 1, 40, flag, prefixString);
    this->readMatrixTriples(40, 40, 20, flag, prefixString);
    this->readMatrixTriples(40, 20, 1, flag, prefixString);
    this->readMatrixTriples(40, 1, 40, flag, prefixString);
    this->readMatrixTriples(40, 1, 58, flag, prefixString);
    cout << "\rTriples OK                  " << endl;
}

//生成三元组
void Triples::triplesGen(int epochsT, int epochsP)
{
    if (this->network.sockSer == -1)
        this->network.init(PORT);
    //生成三元组
    for (int i = 0; i < epochsT; i++)
    {
        creat(1, 1, 1, m1m1m1_counts, i, prefix[0]);
        creat(40, 58, 1, m40m58m1_counts, i, prefix[0]);
        creat(40, 40, 1, m40m40m1_counts, i, prefix[0]);
        creat(20, 40, 1, m20m40m1_counts, i, prefix[0]);
        creat(1, 20, 1, m1m20m1_counts, i, prefix[0]);
        creat(1, 1, 20, m1m1m20_counts, i, prefix[0]);
        creat(20, 20, 1, m20m20m1_counts, i, prefix[0]);
        creat(20, 1, 1, m20m1m1_counts, i, prefix[0]);
        creat(20, 1, 40, m20m1m40_counts, i, prefix[0]);
        creat(40, 40, 20, m40m40m20_counts, i, prefix[0]);
        creat(40, 20, 1, m40m20m1_counts, i, prefix[0]);
        creat(40, 1, 40, m40m1m40_counts, i, prefix[0]);
        creat(40, 1, 58, m40m1m58_counts, i, prefix[0]);
    }
    for (int i = 0; i < epochsP; i++)
    {
        creat(1, 1, 1, m1m1m1_counts, i, prefix[1]);
        creat(40, 58, 1, m40m58m1_counts, i, prefix[1]);
        creat(40, 40, 1, m40m40m1_counts, i, prefix[1]);
        creat(20, 40, 1, m20m40m1_counts, i, prefix[1]);
        creat(1, 20, 1, m1m20m1_counts, i, prefix[1]);
        creat(1, 1, 20, m1m1m20_counts, i, prefix[1]);
        creat(20, 20, 1, m20m20m1_counts, i, prefix[1]);
        creat(20, 1, 1, m20m1m1_counts, i, prefix[1]);
        creat(20, 1, 40, m20m1m40_counts, i, prefix[1]);
        creat(40, 40, 20, m40m40m20_counts, i, prefix[1]);
        creat(40, 20, 1, m40m20m1_counts, i, prefix[1]);
        creat(40, 1, 40, m40m1m40_counts, i, prefix[1]);
        creat(40, 1, 58, m40m1m58_counts, i, prefix[1]);
    }
}

//根据尺寸生成不同的三元组
void Triples::creat(int m, int d, int n, int counts, int flag, string prefixString)
{
    if (this->network.sockSer == -1)
        this->network.init(PORT);
    string fileName = (role == SERVER) ? "SERVER" : "CLIENT";
    fileName = "./input/triples/" + prefixString + fileName;
    if (access("./input/triples/", 0))
        cout << "dir error" << endl;
    if (m == 1 && d == 1 && n == 1)
    {
        fileName += ("_int_" + to_string(flag) + ".trp");
        cout << "Generate " << triples_num[0] << " 1×1×1 triples" << endl;
        ofstream outfile;
        outfile.open(fileName, ios::out | ios::trunc);
        if (!outfile)
        {
            cout << "Open file failed" << endl;
            exit(1);
        }
        for (int i = 0; i < counts; i++)
        {
            this->createIntTriple(outfile);
            cout << "[" << setiosflags(ios::fixed) << setprecision(2) << (i + 1) * 100.0 / counts << "%] "
                 << symbol[i / 10 % 4] << " \r" << flush;
        }
        cout << endl;
        showTime();
        outfile.close();
    }
    else
    {
        fileName += ("_matrix:" + to_string(this->table[m][d][n]) + "_" + to_string(m) + "-" + to_string(d) + "-" +
                     to_string(n) + "_" + to_string(flag) + ".trp");
        cout << "Generate " << triples_num[this->table[m][d][n] + 1] << " " << m << "×" << d << "×" << n << " triples"
             << endl;
        ofstream outfile;
        outfile.open(fileName, ios::out | ios::trunc);
        if (!outfile)
        {
            cout << "Open file failed" << endl;
            exit(1);
        }
        for (int i = 0; i < counts; i++)
        {
            this->createMatrixTriple(outfile, m, d, n);
            cout << "[" << setiosflags(ios::fixed) << setprecision(2) << (i + 1) * 100.0 / counts << "%] "
                 << symbol[i % 4] << " \r" << flush;
        }
        cout << endl;
        showTime();
        outfile.close();
    }
}

//生成普通三元组，并写入文件
void Triples::createIntTriple(ofstream &outfile)
{
    mpz_class a, b, c, aTimesB;
    string index, ck_string = checkMSG, recv_string;
    a = randNumGen();
    b = randNumGen();
    c = randNumGen();
    if (role == SERVER)
    { // SERVER接收对方的R和W，计算u和v发送给对方
        Matrix R, W;
        mpz_class u = 0, v = 0, temp_u, temp_v;
        this->tripleTools.mLocalMocheng(a, b, aTimesB);
        mpz_class array[5] = {aTimesB, a, b, 1, c};
        Matrix X(1, 5, array);
        this->network.mReceive(R);
        this->network.mSend(ck_string);
        this->network.mReceive(W);
        for (int i = 0; i < 5; i++)
        {
            this->tripleTools.mLocalMocheng(R.matrix[0][i], X.matrix[0][i], temp_u);
            this->tripleTools.mAccu(u, temp_u);
            this->tripleTools.mLocalMocheng(W.matrix[0][i], X.matrix[0][i], temp_v);
            this->tripleTools.mAccu(v, temp_v);
        }
        this->network.mSend(u);
        this->network.mReceive(recv_string);
        this->network.mSend(v);
        this->network.mReceive(index);
    }
    else
    { // CLIENT计算R和W发送给对方，并接收对方生成的u和v，计算出c
        Matrix Z, W;
        mpz_class r = randNumGen();
        mpz_class u, v, r_inv, temp;
        this->tripleTools.mLocalMocheng(a, b, aTimesB);
        mpz_class array[5] = {1, b, a, aTimesB, -1};
        Matrix Y(1, 5, array);
        Matrix R(1, 5);
        this->tripleTools.mSub(Y, R, Z);
        this->tripleTools.mConstMulOrigin(Z, W, r.get_mpz_t());
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(u);
        this->network.mSend(ck_string);
        this->network.mReceive(v);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), modNum[modNumIndex].get_mpz_t());
        this->tripleTools.mLocalMocheng(r_inv, v, temp);
        this->tripleTools.mojia(u, temp, c);
        //由于CLIENT计算c，因此在计算完后生成随机序列号发送给对方，作为两方三元组的对应标志
        index = this->randIndex();
        this->network.mSend(index);
    }
    //将三元组保存到文件中
    string out_string, out_a, out_b, out_c;
    network.serialization(a, out_a);
    network.serialization(b, out_b);
    network.serialization(c, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
}

//生成矩阵三元组，并写入文件
void Triples::createMatrixTriple(ofstream &outfile, int m, int d, int n)
{
    string index, ck_string = checkMSG, recv_string;
    mpz_class r_inv;
    Matrix A{m, d}, B{d, n}, C{m, n}, aTimesB;
    Matrix I{M_NORMAL, 1, 1, 1}, I_minus{M_NORMAL, -1, 1, 1};
    this->tripleTools.mLocalMul(A, B, aTimesB);
    if (role == SERVER)
    {
        Matrix R1, R2, R3, R4, R5;
        Matrix W1, W2, W3, W4, W5;
        array<Matrix, 5> R = {R1, R2, R3, R4, R5};
        array<Matrix, 5> W = {W1, W2, W3, W4, W5};
        array<Matrix, 5> X = {aTimesB, A, B, I, C};
        Matrix U{M_NORMAL, 0, m, n}, V{M_NORMAL, 0, m, n}, temp_U, temp_V;
        this->network.mReceive(R);
        this->network.mSend(ck_string);
        this->network.mReceive(W);
        this->tripleTools.mConstMulOrigin(X[0], temp_U, R[0].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(X[1], R[1], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(R[2], X[2], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mConstMulOrigin(R[3], temp_U, X[3].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mConstMulOrigin(X[4], temp_U, R[4].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(U, temp_U);

        this->tripleTools.mConstMulOrigin(X[0], temp_V, W[0].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(X[1], W[1], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(W[2], X[2], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mConstMulOrigin(W[3], temp_V, X[3].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mConstMulOrigin(X[4], temp_V, W[4].matrix[0][0].get_mpz_t());
        this->tripleTools.mAccu(V, temp_V);

        this->network.mSend(U);
        this->network.mReceive(recv_string);
        this->network.mSend(V);
        this->network.mReceive(index);
    }
    else
    {
        array<Matrix, 5> Y = {I, B, A, aTimesB, I_minus};
        Matrix R1{1, 1}, R2{d, n}, R3{m, d}, R4{m, n}, R5{1, 1};
        Matrix W1, W2, W3, W4, W5, temp, U, V;
        Matrix Z1, Z2, Z3, Z4, Z5;
        array<Matrix, 5> R = {R1, R2, R3, R4, R5};
        mpz_class r = randNumGen();
        array<Matrix, 5> Z = {Z1, Z2, Z3, Z4, Z5};
        array<Matrix, 5> W = {W1, W2, W3, W4, W5};
        for (int i = 0; i < 5; i++)
        {
            this->tripleTools.mSub(Y[i], R[i], Z[i]);
            this->tripleTools.mConstMulOrigin(Z[i], W[i], r.get_mpz_t());
        }
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(U);
        this->network.mSend(ck_string);
        this->network.mReceive(V);

        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), modNum[modNumIndex].get_mpz_t());
        Matrix uTimesRinv;
        this->tripleTools.mConstMulOrigin(V, uTimesRinv, r_inv.get_mpz_t());
        this->tripleTools.mAdd(U, uTimesRinv, C);
        //由于CLIENT计算c，因此在计算完后生成随机序列号发送给对方，作为两方三元组的对应标志
        index = this->randIndex();
        this->network.mSend(index);
    }
    //将三元组保存到文件中
    string out_string, out_a, out_b, out_c;
    network.serialization(A, out_a);
    network.serialization(B, out_b);
    network.serialization(C, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
}

//读入文件里的普通三元组
void Triples::readIntTriples(int flag, string prefixString)
{
    string fileName = this->dirTriples + prefixString + ((role == SERVER) ? "SERVER" : "CLIENT") + "_int_" + to_string(flag) + ".trp";
    string fileNameNew = "USED_" + fileName;
    ifstream infile;
    infile.open(fileName, ios::in);
    if (!infile)
    { //三元组文件不存在，则生成
        cout << "\n1×1×1 triples not found" << endl;
        this->creat(1, 1, 1, triples_num[0], flag, prefixString);
        infile.open(fileName, ios::in);
    }
    string line;
    mpz_class index, a, b, c;
    while (!this->intTriples.empty())
        this->intTriples.pop();
    // cout << "stack size: " << this->intTriples.size() << endl;
    while (getline(infile, line) && infile.good() && !infile.eof() && line != "")
    {
        this->deserialization(line, index, a, b, c);
        IntTriples triples{index, a, b, c};
        this->intTriples.push(triples);
    }
    infile.close();
    //已读取的文件进行改名
    if (RENAME)
        if (rename(fileName.c_str(), fileNameNew.c_str()) == -1)
            cout << "Rename error" << endl;
}

//读入文件里的矩阵三元组
void Triples::readMatrixTriples(int m, int d, int n, int flag, string prefixString)
{
    int indexFlag = this->table[m][d][n];
    string fileName = this->dirTriples + prefixString + ((role == SERVER) ? "SERVER" : "CLIENT") + "_matrix:" + to_string(indexFlag) + "_" + to_string(m) + "-" + to_string(d) + "-" + to_string(n) + "_" + to_string(flag) + ".trp";
    string fileNameNew = "USED_" + fileName;
    ifstream infile;
    infile.open(fileName, ios::in);
    if (!infile)
    { //三元组文件不存在，则生成
        cout << "\n"
             << m << "×" << d << "×" << n << " triples not found" << endl;
        this->creat(m, d, n, triples_num[indexFlag + 1], flag, prefixString);
        infile.open(fileName, ios::in);
    }
    string line;
    Matrix a, b, c;
    mpz_class index;
    while (!this->matrixTriples[indexFlag].empty())
        this->matrixTriples[indexFlag].pop();
    while (getline(infile, line) && infile.good() && !infile.eof() && line != "")
    {
        this->deserialization(line, index, a, b, c);
        MatrixTriples triples{index, a, b, c};
        this->matrixTriples[indexFlag].push(triples);
    }
    infile.close();
    //已读取的文件进行改名
    if (RENAME)
        if (rename(fileName.c_str(), fileNameNew.c_str()) == -1)
            cout << "Rename error" << endl;
}

//三元组反序列化
bool Triples::deserialization(string in_string, mpz_class &index, mpz_class &a, mpz_class &b, mpz_class &c)
{
    char *pch;
    char *cstr = stringToChar(in_string);
    pch = strtok(cstr, iDelim);
    index = mpz_class(pch, baseNum); //分离出序号
    pch = strtok(nullptr, iDelim);   //剩下的三元组
    pch = strtok(pch, tDelim);
    a = mpz_class(pch, baseNum); //分离出a
    pch = strtok(nullptr, tDelim);
    b = mpz_class(pch, baseNum); //分离出b
    pch = strtok(nullptr, tDelim);
    c = mpz_class(pch, baseNum); //分离出c
    delete[] cstr;
    return true;
}

bool Triples::deserialization(string in_string, mpz_class &index, Matrix &a, Matrix &b, Matrix &c)
{
    char *pch1, *pch2;
    char *cstr = stringToChar(in_string);
    pch1 = strtok(cstr, iDelim);
    index = mpz_class(pch1, baseNum); //分离出序号
    pch1 = strtok(nullptr, iDelim);   //剩下的三元组
    pch2 = strtok(pch1, tDelim);
    string a_string = pch2; //分离出a
    pch2 = strtok(nullptr, tDelim);
    string b_string = pch2; //分离出b
    pch2 = strtok(nullptr, tDelim);
    string c_string = pch2; //分离出c
    network.deserialization(a_string, a);
    network.deserialization(b_string, b);
    network.deserialization(c_string, c);
    delete[] cstr;
    return true;
}

//获取一个普通三元组
IntTriples Triples::getTriples()
{
    if (this->intTriples.empty())
    {
        cout << "ERROR: Int triples run out!" << endl;
        exit(1);
    }
    IntTriples triples = this->intTriples.top();
    if (RELEASE)
        this->intTriples.pop();
    else
        m1m1m1_num++;
    return triples;
}

//获取一个矩阵三元组
MatrixTriples Triples::getTriples(int m, int d, int n)
{
    int flag = this->table[m][d][n];
    if (this->matrixTriples[flag].empty())
    {
        cout << "ERROR: " << m << "×" << d << "×" << n << " matrix triples run out!" << endl;
        exit(1);
    }
    MatrixTriples triples = this->matrixTriples[flag].top();
    if (RELEASE)
        this->matrixTriples[flag].pop();
    else
    {
        mTriples_num[flag]++;
        if (!(triples.a.row == m &&
              triples.a.col == d &&
              triples.b.row == d &&
              triples.b.col == n &&
              triples.c.row == m &&
              triples.c.col == n))
        {
            cout << "ERROR: Get " << m << "×" << d << "×" << n << " matrix triples failed!" << endl;
            exit(1);
        }
    }

    return triples;
}

//生成随机序列号
string Triples::randIndex()
{
    time_t times = time(nullptr);
    clock_t clocks = clock();
    int randBit = rand() % 10;
    string index = to_string(times) + to_string(clocks) + to_string(randBit);
    return index;
}