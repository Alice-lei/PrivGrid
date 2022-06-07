#include "mulgate.h"

using namespace std;

typedef struct TripleNode
{
    IntTriples triple;
    mpz_class E, F;
} Triple;

class TripleList
{
public:
    void pushTriple(Triple triple);

    void popTriple(Triple &triple);

private:
    long unsigned int index = 0;
    vector<Triple> tripleList;
};

void TripleList::pushTriple(Triple triple)
{
    this->tripleList.push_back(triple);
}

void TripleList::popTriple(Triple &triple)
{
    if ((!this->tripleList.empty()) && index < this->tripleList.size())
        triple = this->tripleList[index++];
    else
        cout << "Triple list pop failed!" << endl;
}

//初始化
void TriplesMul::init(int flag, string prefixString,string dirTriples)
{
    if (!flag)
    {
        if (this->network.sockSer == -1)
            this->network.init(PORT+1);
    }
    MulTriples.init(flag, prefixString,dirTriples);
}

//矩阵三元组乘法
void TriplesMul::mMul(Matrix x, Matrix y, Matrix &ans)
{ //得多次调用对应尺寸的矩阵乘法三元组
    assert(x.col == y.row);
    if (x.row == 1 && x.col == 1 && y.col == 1)
    {
        mocheng(x.matrix[0][0], y.matrix[0][0], ans.matrix[0][0]);
        return;
    }
    Matrix Ex, Ey, E, Fx, Fy, F, temp;
    string ck_string = checkMSG, recv_string;
    this->mulgateTools.mResize(x.row, y.col, ans);
    MatrixTriples triad = MulTriples.getTriples(x.row, x.col, y.col);
    this->mulgateTools.mSub(x, triad.a, Ex);
    this->mulgateTools.mSub(y, triad.b, Fx);
    if (role == SERVER)
    {
        this->network.mSend(Ex, Fx);
        this->network.mReceive(Ey, Fy);
    } else
    {
        this->network.mReceive(Ey, Fy);
        this->network.mSend(Ex, Fx);
    }
    this->mulgateTools.mAdd(Ex, Ey, E);
    this->mulgateTools.mAdd(Fx, Fy, F);
    if (role == SERVER)
    {
        this->mulgateTools.mLocalMul(E, F, temp);
        this->mulgateTools.mAccu(ans, temp);
    }
    this->mulgateTools.mLocalMul(triad.a, F, temp);
    this->mulgateTools.mAccu(ans, temp);
    this->mulgateTools.mLocalMul(E, triad.b, temp);
    this->mulgateTools.mAccu(ans, temp);
    this->mulgateTools.mAccu(ans, triad.c);

    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mpz_class z = ans.matrix[i][j];
            if (z != 0)
            {
                if (role == SERVER)
                {
                    mpz_class temp_div;
                    temp_div = modNum[modNumIndex] - z; //先模减
                    mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
                    z = modNum[modNumIndex] - z; //再模减
                } else
                {
                    mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小2^32倍
                }
            }
            ans.matrix[i][j] = z;
        }
    }
}

//三元组模乘，计算后缩小
void TriplesMul::mocheng(mpz_class x, mpz_class y, mpz_class &z)
{
    mpz_class Ex, Ey, E, Fx, Fy, F;
    mpz_class temp1, temp2, temp3, temp4 = 0;
    IntTriples triad = MulTriples.getTriples();
    //三元组乘法
    this->mulgateTools.mojian(x, triad.a, Ex);
    this->mulgateTools.mojian(y, triad.b, Fx);
    if (role == SERVER)
    {
        this->network.mSend(Ex, Fx);
        this->network.mReceive(Ey, Fy);
    } else
    {
        this->network.mReceive(Ey, Fy);
        this->network.mSend(Ex, Fx);
    }
    this->mulgateTools.mojia(Ex, Ey, E);
    this->mulgateTools.mojia(Fx, Fy, F);
    if (role == SERVER)
        mpz_mul(temp4.get_mpz_t(), E.get_mpz_t(), F.get_mpz_t());
    mpz_mul(temp1.get_mpz_t(), F.get_mpz_t(), triad.a.get_mpz_t());
    mpz_mul(temp2.get_mpz_t(), E.get_mpz_t(), triad.b.get_mpz_t());
    temp3 = triad.c;
    z = temp1 + temp2 + temp3 + temp4;
    mpz_mod(z.get_mpz_t(), z.get_mpz_t(), modNum[modNumIndex].get_mpz_t()); //取模
    //截断操作
    if (z != 0)
    {
        if (role == SERVER)
        {
            mpz_class temp_div;
            temp_div = modNum[modNumIndex] - z; //先模减
            mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
            z = modNum[modNumIndex] - z;                                              //再模减
        } else
            mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小2^32倍
    }
}

//矩阵三元组LSTM乘法
void TriplesMul::mMull(Matrix x, Matrix y, Matrix &ans)
{ //得多次调用1*1乘法三元组
    assert(x.col == y.col && x.row == y.row);
    this->mulgateTools.mResize(x.row, x.col, ans);
    string send_string, recv_string;
    TripleList tList;
    //get triples and sub, get Ex and Fx
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            Triple temp;
            temp.triple = MulTriples.getTriples();
            //三元组乘法
            this->mulgateTools.mojian(x.matrix[i][j], temp.triple.a, temp.E);
            this->mulgateTools.mojian(y.matrix[i][j], temp.triple.b, temp.F);
            send_string += (temp.E.get_str() + mDelim + temp.F.get_str());
            tList.pushTriple(temp);
            if ((i < ans.row - 1) || (j < ans.col - 1))
                send_string += xDelim;
        }
    }
    //Netwoek
    if (role == SERVER)
    {
        this->network.mSend(send_string);
        this->network.mReceive(recv_string);
    } else
    {
        this->network.mReceive(recv_string);
        this->network.mSend(send_string);
    }
    char *cstr = stringToChar(recv_string);
    //add to get E and F, and other
    char *restX = nullptr;
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            Triple temp;
            mpz_class temp1, temp2, temp3, temp4 = 0, z;
            char *restM = nullptr;
            char *pch, *pch1, *pch2;
            if (i == 0 && j == 0)
                pch = strtok_r(cstr, xDelim, &restX);
            else
                pch = strtok_r(nullptr, xDelim, &restX);
            pch1 = strtok_r(pch, mDelim, &restM);
            pch2 = strtok_r(nullptr, mDelim, &restM);
            mpz_class Ey{pch1}, Fy{pch2};
            tList.popTriple(temp);
            this->mulgateTools.mojia(temp.E, Ey, temp.E);
            this->mulgateTools.mojia(temp.F, Fy, temp.F);
            if (role == SERVER)
                mpz_mul(temp4.get_mpz_t(), temp.E.get_mpz_t(), temp.F.get_mpz_t());
            mpz_mul(temp1.get_mpz_t(), temp.F.get_mpz_t(), temp.triple.a.get_mpz_t());
            mpz_mul(temp2.get_mpz_t(), temp.E.get_mpz_t(), temp.triple.b.get_mpz_t());
            temp3 = temp.triple.c;
            z = temp1 + temp2 + temp3 + temp4;
            mpz_mod(z.get_mpz_t(), z.get_mpz_t(), modNum[modNumIndex].get_mpz_t()); //取模
            //截断操作
            if (z != 0)
            {
                if (role == SERVER)
                {
                    mpz_class temp_div;
                    temp_div = modNum[modNumIndex] - z; //先模减
                    mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
                    z = modNum[modNumIndex] - z;                                              //再模减
                } else
                    mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小2^32倍
            }
            ans.matrix[i][j] = z;
        }
    }
    delete[] cstr;
}

//矩阵三元组并列乘方
void TriplesMul::mPoww(Matrix x, Matrix &ans)
{
    this->mMull(x, x, ans);
}

//矩阵常数乘法，截断
void TriplesMul::mConstMul(Matrix x, Matrix &ans, mpz_ptr num)
{
    this->mulgateTools.mResize(x.row, x.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < x.col; j++)
        {
            mpz_class zz(num);
            this->mulgateTools.mLocalMocheng(x.matrix[i][j], zz, ans.matrix[i][j]); //使用本地模乘
            mpz_class z = ans.matrix[i][j];
            if (z != 0)
            {
                if (role == SERVER)
                {
                    mpz_class temp_div;
                    temp_div = modNum[modNumIndex] - z; //先模减
                    mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
                    z = modNum[modNumIndex] - z;                                              //再模减
                } else
                    mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小
            }
            ans.matrix[i][j] = z;
        }
    }
}

void TriplesMul::sigmoid(Matrix &matrix)
{ //a+bx+cx^2+dx^3
    Matrix powerAns1, powerAns2, addAns1, addAns2, addAns3;
    Matrix onceAns, twiceAns, thriceAns, msig_0;
    if (role==SERVER)
    {
        Matrix temp{M_NORMAL, sig0, matrix.row, matrix.col}; //零次项系数,a
        mulgateTools.mCopy(temp, msig_0);
    } else
    {
        Matrix temp{M_NORMAL, 0, matrix.row, matrix.col}; //零次项系数,a
        mulgateTools.mCopy(temp, msig_0);
    }
    this->mConstMul(matrix, onceAns, sig1.get_mpz_t()); //乘一次项系数，bx
    this->mPoww(matrix, powerAns1); //二次项，x^2
    this->mConstMul(powerAns1, twiceAns, sig2.get_mpz_t()); //二次项系数，cx^2
    this->mMull(matrix, powerAns1, powerAns2); //三次项，x^3
    this->mConstMul(powerAns2, thriceAns, sig3.get_mpz_t()); //三次项系数，dx^3
    mulgateTools.mAdd(thriceAns, twiceAns, addAns1); //三次加二次，cx^2 + dx^3
    mulgateTools.mAdd(addAns1, onceAns, addAns2);    //再加一次，bx + cx^2 + dx^3
    mulgateTools.mAdd(addAns2, msig_0, addAns3);     //再加零次，a + bx + cx^2 + dx^3
    mulgateTools.mCopy(addAns3, matrix);             //结果
}

void TriplesMul::tanh(Matrix matrix, Matrix &ans)
{
    Matrix temp{M_NORMAL, 0, matrix.row, matrix.col}, temp2{M_NORMAL, 0, matrix.row, matrix.col};
    Matrix const_one(M_CIPHER, 1, matrix.row, matrix.col);
    mpz_class two = 2;
    mulgateTools.mConstMulOrigin(matrix, temp, two.get_mpz_t());
    this->sigmoid(temp);
    mulgateTools.mConstMulOrigin(temp, temp2, two.get_mpz_t());
    mulgateTools.mSub(temp2, const_one, matrix);
    mulgateTools.mCopy(matrix, ans);
}

//恢复矩阵明文结果
void TriplesMul::getPlain(Matrix cipher, string outputWord)
{
    Matrix cipher1, cipher2, temp;
    this->mulgateTools.mCopy(cipher, cipher1);
    if (role == SERVER)
    {
        this->network.mSend(cipher1);
        this->network.mReceive(cipher2);
    } else
    {
        this->network.mReceive(cipher2);
        this->network.mSend(cipher1);
    }
    this->mulgateTools.mAdd(cipher1, cipher2, temp);
    cout << outputWord << ": ";
    if (!(cipher.row == 1 && cipher.col == 1))
        cout << endl;
    for (int i = 0; i < temp.row; i++)
    {
        for (int j = 0; j < temp.col; j++)
        {
            mpz_class tempZ, tempMod;
            mpf_class tempF;
            if (temp.matrix[i][j].get_d() != 0)
            {
                mpf_class tempF1{temp.matrix[i][j]};
                if (tempF1 > modNum[modNumIndex] / 2)
                    tempF1 -= modNum[modNumIndex];
                mpf_div_2exp(tempF.get_mpf_t(), tempF1.get_mpf_t(), eAndC);
                gmp_printf("(%2d-%2d) %9.5Ff\t", i, j, tempF.get_mpf_t());
            } else
                gmp_printf("(%2d-%2d) %9Zd\t", i, j, temp.matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

//恢复数明文结果
mpf_class TriplesMul::getPlain(mpz_class cipher, string outputWord)
{
    mpz_class cipher2, temp;
    if (role == SERVER)
    {
        this->network.mSend(cipher);
        this->network.mReceive(cipher2);
    } else
    {
        this->network.mReceive(cipher2);
        this->network.mSend(cipher);
    }
    temp = (cipher + cipher2);
    temp = temp % modNum[modNumIndex];
    cout << outputWord << ": ";
    mpf_class tempF{temp};
    if (tempF != 0)
    {
        if (tempF > modNum[modNumIndex] / 2)
            tempF -= modNum[modNumIndex];
        mpf_div_2exp(tempF.get_mpf_t(), tempF.get_mpf_t(), eAndC);
        gmp_printf("%.8Ff\n", tempF.get_mpf_t());
    } else
        gmp_printf("%.8Ff\n", tempF.get_mpf_t());
    return tempF;
}