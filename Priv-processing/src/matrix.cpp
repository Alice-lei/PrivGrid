#include "matrix.h"

Matrix::Matrix()
{
    this->col = this->row = 0;
}

//生成随机矩阵
Matrix::Matrix(int row, int col)
{ //慢
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
            this->matrix[i][j] = randNumGen();
    }
}

//生成特殊分布的矩阵
Matrix::Matrix(bool flag, int row, int col)
{
    this->row = row;
    this->col = col;
    random_device rd;
    default_random_engine rng{rd()};
    //uniform_real_distribution<double> distribution(-0.1, 0.1);//均匀分布
    normal_distribution<double> distribution(0, 0.05);//高斯分布
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            double randNum;
            while ((randNum = distribution(rng)) > 1.0 || randNum < -1.0);
            mpf_class num = randNum;
            mpf_mul_2exp(num.get_mpf_t(), num.get_mpf_t(), eAndC);
            this->matrix[i][j] = num;
        }
    }
}

//生成满矩阵或对角矩阵或加密运算满矩阵
Matrix::Matrix(int flag, mpz_class num, int row, int col)
{
    this->row = row;
    this->col = col;
    mpz_class zero = 0;
    mpz_class temp;
    mpz_mul_2exp(temp.get_mpz_t(), num.get_mpz_t(), eAndC);
    mpz_div_2exp(temp.get_mpz_t(), temp.get_mpz_t(), 1);
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            switch (flag)
            {
                case M_NORMAL:
                    this->matrix[i][j] = mpz_class(num);
                    break;
                case M_DIAGONAL:
                    if (j == i)
                        this->matrix[i][j] = mpz_class(num);
                    else
                        this->matrix[i][j] = mpz_class(zero);
                    break;
                case M_CIPHER:
                    this->matrix[i][j] = mpz_class(temp);
                    break;
                default:
                    break;
            }
        }
    }
}

//生成指定内容的矩阵
Matrix::Matrix(int row, int col, int *array)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            this->matrix[i][j] = mpz_class(*array);
            array++;
        }
    }
}

//生成指定内容的矩阵
Matrix::Matrix(int row, int col, mpz_class *array)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            this->matrix[i][j] = *array;
            array++;
        }
    }
}

//将第positionRow行，第positionCol列的数替换为num
void Matrix::change(int positionRow, int positionCol, mpz_class num)
{
    mpz_set(this->matrix[positionRow][positionCol].get_mpz_t(), num.get_mpz_t());
}

//矩阵输出
void Matrix::print()
{
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            gmp_printf("%Zd	", this->matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

void Matrix::print(string out_word)
{
    cout << out_word << ": ";
    if (!(this->row == 1 && this->col == 1))
        cout << endl;
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            gmp_printf("(%2d-%2d) %3Zd\t", i, j, this->matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

//矩阵序列化
string Matrix::serialization()
{
    string matrix_string;
    string matrix_string_row = to_string(this->row);
    string matrix_string_col = to_string(this->col);
    matrix_string = matrix_string_row + mDelim + matrix_string_col + mDelim;
    for (int mRow = 0; mRow < this->row; mRow++)
    {
        for (int mCol = 0; mCol < this->col; mCol++)
        {
            string temp = this->matrix[mRow][mCol].get_str();
            matrix_string += temp;
            if ((mRow < this->row - 1) || (mCol < this->col - 1))
                matrix_string += mDelim;
        }
    }
    return matrix_string;
}

//矩阵反序列化
void Matrix::deserialization(string matrix_ser)
{
    char *pch;
    char *cstr = stringToChar(matrix_ser);
    pch = strtok(cstr, mDelim);
    if (pch == nullptr)
    {
        printf("Deserialization matrix error!(1) : %s\n", strerror(errno));
        exit(1);
    }
    this->row = stoi(pch);
    pch = strtok(nullptr, mDelim);
    if (pch == nullptr)
    {
        printf("Deserialization matrix error!(2) : %s\n", strerror(errno));
        exit(1);
    }
    this->col = stoi(pch);
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            pch = strtok(nullptr, mDelim);
            if (pch == nullptr)
            {
                cout << "Deserialization exchange error!(2)" << endl;
                exit(1);
            }
            this->matrix[i][j] = mpz_class(pch, 10);
        }
    }
    delete[] cstr;
}

//矩阵输出
void MatrixTools::print(Matrix m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            gmp_printf("%Zd	", m.matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

//矩阵复制
void MatrixTools::mCopy(Matrix from, Matrix &to)
{
    this->mResize(from.row, from.col, to);
    for (int i = 0; i < from.row; i++)
    {
        for (int j = 0; j < from.col; j++)
        {
            to.matrix[i][j] = from.matrix[i][j];
        }
    }
}

//矩阵转置
void MatrixTools::mTrans(Matrix from, Matrix &to)
{
    this->mResize(from.col, from.row, to);
    for (int i = 0; i < to.row; i++)
    {
        for (int j = 0; j < to.col; j++)
        {
            to.matrix[i][j] = from.matrix[j][i];
        }
    }
}

//向量截取，舍弃向量from的前trunNum个元素，截取后面的部分存于向量ans中
void MatrixTools::vTrun(int trunNum, Matrix from, Matrix &to)
{
    this->mResize(1, from.col - trunNum, to);
    for (int i = 0; i < to.col; i++)
    {
        to.matrix[0][i] = from.matrix[0][i + trunNum];
    }
}

//向量拼接
void MatrixTools::vConcat(Matrix x, Matrix y, Matrix &ans)
{
    this->mResize(1, x.col + y.col, ans);
    int i, j;
    j = x.col;
    for (i = 0; i < x.col; i++)
    {
        ans.matrix[0][i] = x.matrix[0][i];
    }
    for (i = 0; i < y.col; i++)
    {
        ans.matrix[0][i + j] = y.matrix[0][i];
    }
}

//矩阵加法
void MatrixTools::mAdd(Matrix x, Matrix y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mojia(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
        }
    }
}

//矩阵累加,x+=y
void MatrixTools::mAccu(Matrix &x, Matrix y)
{
    Matrix temp;
    this->mAdd(x, y, temp);
    this->mCopy(temp, x);
}

//矩阵累减,x-=y
void MatrixTools::mAccuSub(Matrix &x, Matrix y)
{
    Matrix temp;
    this->mSub(x, y, temp);
    this->mCopy(temp, x);
}

//矩阵常数乘法，不截断
void MatrixTools::mConstMulOrigin(Matrix x, Matrix &ans, mpz_ptr num)
{
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < x.col; j++)
        {
            mpz_class zz(num);
            this->mLocalMocheng(x.matrix[i][j], zz, ans.matrix[i][j]); //使用本地模乘
        }
    }
}

//矩阵减法
void MatrixTools::mSub(Matrix x, Matrix y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mpz_class a, b;
            a = -1;
            this->mLocalMocheng(a, y.matrix[i][j], b); //使用本地模乘
            this->mojia(x.matrix[i][j], b, ans.matrix[i][j]);
        }
    }
}

//矩阵本地乘法，用于三元组生成
void MatrixTools::mLocalMul(Matrix x, Matrix y, Matrix &ans)
{
    assert(x.col == y.row);
    this->mResize(x.row, y.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < y.col; j++)
        {
            mpz_class sum1, sum4, sum5;
            sum1 = 0;
            sum4 = 0;
            for (int m = 0; m < y.row; m++)
            {
                this->mLocalMocheng(x.matrix[i][m], y.matrix[m][j], sum1); //使用本地模乘
                this->mojia(sum1, sum4, sum5);
                sum4 = sum5;
            }
            ans.matrix[i][j] = sum4;
        }
    }
}

//矩阵三元组LSTM乘法
void MatrixTools::mLocalMull(Matrix x, Matrix y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            this->mLocalMocheng(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
        }
    }
}

//矩阵尺寸重设
void MatrixTools::mResize(int row, int col, Matrix &matrix)
{
    if (matrix.col != col || matrix.row != row)
    {
        matrix.row = row;
        matrix.col = col;
        matrix.matrix.resize(matrix.row);
        for (int i = 0; i < matrix.row; i++)
        {
            vector<mpz_class>(matrix.matrix[i]).swap(matrix.matrix[i]);
            matrix.matrix[i].resize(matrix.col);
            for (int j = 0; j < matrix.col; j++)
                matrix.matrix[i][j] = mpz_class("0", baseNum);
        }
    }
}

//向量扩展为对角矩阵
void MatrixTools::mVector2Matrix(Matrix vector, Matrix &matrix)
{
    assert(vector.col == 1);
    this->mResize(vector.row, vector.row, matrix);
    for (int i = 0; i < vector.row; i++)
        matrix.change(i, i, vector.matrix[i][0]);
}

//矩阵比较，相同返回true，不同返回false
bool MatrixTools::mCmp(Matrix x, Matrix y)
{
    if (x.col != y.col || x.row != y.row)
        return false;
    for (int i = 0; i < x.row; i++)
        for (int j = 0; j < x.col; j++)
        {
            mpz_class ans;
            this->mojian(x.matrix[i][j], y.matrix[i][j], ans);
            if (ans != 0)
                return false;
        }
    return true;
}

//模加
void MatrixTools::mojia(mpz_class x, mpz_class y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_inits(a, b, r, nullptr);
    mpz_add(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum[modNumIndex].get_mpz_t());
    mpz_mod(r, b, modNum[modNumIndex].get_mpz_t());
    z = mpz_class(r);
    mpz_clears(a, b, r, nullptr);
}

//模累加,x+=y
void MatrixTools::mAccu(mpz_class &x, mpz_class y)
{
    this->mojia(x, y, x);
}

//模减
void MatrixTools::mojian(mpz_class x, mpz_class y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_inits(a, b, r, nullptr);
    mpz_sub(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum[modNumIndex].get_mpz_t());
    mpz_mod(r, b, modNum[modNumIndex].get_mpz_t());
    z = mpz_class(r);
    mpz_clears(a, b, r, nullptr);
}

//本地模乘
void MatrixTools::mLocalMocheng(mpz_class x, mpz_class y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_inits(a, b, r, nullptr);
    mpz_mul(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum[modNumIndex].get_mpz_t());
    mpz_mod(r, b, modNum[modNumIndex].get_mpz_t());
    z = mpz_class(r);
    mpz_clears(a, b, r, nullptr);
}