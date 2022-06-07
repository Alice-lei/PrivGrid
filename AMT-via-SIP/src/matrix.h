#ifndef __MATRIX__
#define __MATRIX__

#include "global.h"

class Matrix
{
public:
    int col;
    int row;
    vector<vector<mpz_class>> matrix;

    Matrix();

    Matrix(int row, int col);

    Matrix(int row, int col, int *array);

    Matrix(int row, int col, mpz_class *array);

    Matrix(bool flag, int row, int col);

    Matrix(int flag, mpz_class num, int row, int col);

    void print(); //矩阵输出
    void print(string out_word);
    string serialization();
    void deserialization(string matrix_ser);
    void change(int positionRow, int positionCol, mpz_class num);

private:
    void CreateMatrix(int row, int col);          //创建随机矩阵
    void CreateMatrix(int num, int row, int col); //创建对角矩阵
};

class MatrixTools
{
public:
    void print(Matrix m);                              //矩阵输出
    void mCopy(Matrix from, Matrix &to);               //矩阵复制
    void mTrans(Matrix from, Matrix &to);              //矩阵转置
    void vTrun(int turnNum, Matrix from, Matrix &to);  //向量截取
    void vConcat(Matrix x, Matrix y, Matrix &ans);    //向量拼接
    void mAdd(Matrix x, Matrix y, Matrix &ans);       //矩阵加法
    void mAccu(Matrix &x, Matrix y);                   //矩阵累加,x+=y
    void mAccu(mpz_class &x, mpz_class y);

    void mAccuSub(Matrix &x, Matrix y);                //矩阵累减,x-=y
    void mConstMulF(Matrix x, Matrix &ans, float num);  //矩阵常数乘法
    //void mConstMul(Matrix x, Matrix &ans, mpz_ptr num); //矩阵常数乘法
    void mConstMulOrigin(Matrix x, Matrix &ans, mpz_ptr num);

    void mSub(Matrix x, Matrix y, Matrix &ans);       //矩阵减法
    void mLocalMul(Matrix x, Matrix y, Matrix &ans);  //矩阵本地乘法
    void mLocalMull(Matrix x, Matrix y, Matrix &ans); //矩阵本地并列乘法
    void mVector2Matrix(Matrix vector, Matrix &matrix);

    void mojia(mpz_class x, mpz_class y, mpz_class &z);

    void mojian(mpz_class x, mpz_class y, mpz_class &z);

    void mLocalMocheng(mpz_class x, mpz_class y, mpz_class &z);

    void mResize(int row, int col, Matrix &y);

    bool mCmp(Matrix matrix_one, Matrix matrix_two); //矩阵比较，相同返回true，不同返回false
};

#endif