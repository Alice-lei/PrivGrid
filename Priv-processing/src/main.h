#include "share.h"
#include "mulgate.h"
#include "network.h"
#include "triplescheck.h"
#include "plainmatrix.h"

using namespace std;
TriplesMul triplesMul;
MatrixTools lstmTools;
#define mx                 \
    {                      \
        M_NORMAL, 0, 58, 1 \
    }
#define mo                 \
    {                      \
        M_NORMAL, 0, 40, 1 \
    }

class Lstm;

class Lstm_layer1;

class Lstm_layer2;

class Lstm_layer3;

class Lstm_layer4;

class Lstm_layer_block;

class Lstm_layer_block
{
public:
    Matrix x{M_NORMAL, 0, 58, 1};
    Matrix h{M_NORMAL, 0, 40, 1};
    Matrix f{M_NORMAL, 0, 40, 1};
    Matrix i{M_NORMAL, 0, 40, 1};
    Matrix g{M_NORMAL, 0, 40, 1};
    Matrix o{M_NORMAL, 0, 40, 1};
};

class Lstm_layer4
{
public:
    Matrix W4{M_GAUSS, 1, 20}, B4{M_NORMAL, 0, 1, 1};
    Matrix A4{M_NORMAL, 0, 1, 1}, Z4{M_NORMAL, 0, 1, 1};

    void forward(Lstm_layer3 layer3);

    void backward(Lstm_layer3 layer3);

    Matrix e_t{M_NORMAL, 0, 1, 1};
    Matrix delta_b4{M_NORMAL, 0, 1, 1};
};

class Lstm_layer3
{
public:
    Matrix W3{M_GAUSS, 20, 40}, B3{M_NORMAL, 0, 20, 1};
    Matrix A3{M_NORMAL, 0, 20, 1}, Z3{M_NORMAL, 0, 20, 1};

    void forward(Lstm_layer2 layer2);

    void backward(Lstm_layer4 layer4, Lstm_layer2 layer2);

    Matrix delta_b3{M_NORMAL, 0, 20, 1};
};

class Lstm_layer2
{
public:
    Matrix H[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix S[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix F[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix I[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix G[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix O[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix delta_Bf[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bi[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bg[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bo[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix h_minus1{M_NORMAL, 0, 40, 1};
    Matrix s_minus1{M_NORMAL, 0, 40, 1};
    Matrix Z2{M_NORMAL, 0, 40, 1};
    Matrix A2{M_NORMAL, 0, 40, 1};
    Matrix X20{M_NORMAL, 0, 40, 1};
    Matrix Wfx2{M_GAUSS, 40, 40}, Wfh2{M_GAUSS, 40, 40}, Bf2{M_NORMAL, 0, 40, 1};
    Matrix Wix2{M_GAUSS, 40, 40}, Wih2{M_GAUSS, 40, 40}, Bi2{M_NORMAL, 0, 40, 1};
    Matrix Wgx2{M_GAUSS, 40, 40}, Wgh2{M_GAUSS, 40, 40}, Bg2{M_NORMAL, 0, 40, 1};
    Matrix Wox2{M_GAUSS, 40, 40}, Woh2{M_GAUSS, 40, 40}, Bo2{M_NORMAL, 0, 40, 1};

    Lstm_layer_block block;

    void forward(Lstm_layer1 layer1);

    void backward(Lstm_layer3 layer3, Lstm_layer1 layer1);
};

class Lstm_layer1
{
public:
    Matrix X[20]{mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx};
    Matrix H[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix S[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix F[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix I[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix G[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix O[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix h_minus1{M_NORMAL, 0, 40, 1};
    Matrix s_minus1{M_NORMAL, 0, 40, 1};
    Matrix Wfx1{M_GAUSS, 40, 58}, Wfh1{M_GAUSS, 40, 40}, Bf1{M_NORMAL, 0, 40, 1};
    Matrix Wix1{M_GAUSS, 40, 58}, Wih1{M_GAUSS, 40, 40}, Bi1{M_NORMAL, 0, 40, 1};
    Matrix Wgx1{M_GAUSS, 40, 58}, Wgh1{M_GAUSS, 40, 40}, Bg1{M_NORMAL, 0, 40, 1};
    Matrix Wox1{M_GAUSS, 40, 58}, Woh1{M_GAUSS, 40, 40}, Bo1{M_NORMAL, 0, 40, 1};
    Lstm_layer_block block;

    void forward();

    void backward(Lstm_layer2 layer2);
};

class Lstm
{
public:
    Lstm_layer1 layer1;
    Lstm_layer2 layer2;
    Lstm_layer3 layer3;
    Lstm_layer4 layer4;
    Matrix output{M_NORMAL, 0, 1, 1};
    Matrix output_train{M_NORMAL, 0, 1, 1};

    Lstm();

    void train(string dirData, string dirTriples);

    void test(int userIndex, FILE *fptr, FILE *fptrT);

    void tripesInit(int flag, string prefixString);

    void triplesGen(int epochsT, int epochsP);

private:
    mpf_class actualValue, predictiveValue, deltaValue;
    string dirTriples;
    int counts = 0;
    vector<array<string, 58>> data;
    void dataReadIn(int dataSetindex, int userIndex);
    bool dataCk(string filePath);
    void dataFill(int index, bool flag);
    void forwardNetwork();
    void backwardNetwork();
    void errorDisplay();
    void saveMatrix(string fileName);
    void loadMatrix(string fileName);
};

Lstm::Lstm()
{
}

//三元组生成
void Lstm::triplesGen(int epochsT, int epochsP)
{
    Triples triples;
    cout << "\nTriples generat start!" << endl;
    triples.triplesGen(epochsT, epochsP);
    cout << "\nTriples generat done!" << endl;
}

//数据读入
void Lstm::dataReadIn(int dataSetindex, int userIndex)
{
    cout << "Data preparing" << endl;
    //读取数据
    this->data.clear();
    this->data.shrink_to_fit();
    ifstream infile;
    string line;
    string fileName;
    fileName = (role == SERVER) ? "SERVER" : "CLIENT";
    fileName = "input/data/" + fileName + "_" + to_string(dataSetindex) + ".dat";
    infile.open(fileName, ios::in);
    char *pch;
    int index = 0;
    while ((index < userIndex * 480) && getline(infile, line))
        ++index;
    index = 0;
    while (index < 480 && getline(infile, line))
    {
        char *cstr = stringToChar(line);
        array<string, 58> dataTemp;
        for (int j = 0; j < 58; ++j)
        {
            if (j)
                pch = strtok(nullptr, mDelim);
            else
                pch = strtok(cstr, mDelim);
            dataTemp[j] = pch;
        }
        this->data.push_back(dataTemp);
        delete[] cstr;
        ++index;
    }
    infile.close();
}

//数据检查
bool Lstm::dataCk(string filePath)
{
    cout << "Data check: " << filePath << endl;
    ifstream infile;
    string line;
    infile.open(filePath, ios::in);
    this->counts = 0;
    while (getline(infile, line))
        ++this->counts;
    infile.close();
    if (this->counts % 480)
    { //数据错误，不是480的整数倍
        cout << "Data error!" << endl;
        return false;
    }
    cout << "There are a total of " << this->counts << " data." << endl;
    return true;
}

//装填训练数据
void Lstm::dataFill(int index, bool flag)
{
    int indexStart;
    if (flag == TRAIN)
        indexStart = index * steps;
    else
        indexStart = 432 + index * steps;
    int indexMax = this->data.size();
    // cout << "indexStart: " << indexStart << endl;
    if (indexStart + 21 > indexMax)
    {
        cout << "Data fill failed!" << endl;
        exit(1);
    }
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 58; j++)
        {
            mpz_class temp{this->data[indexStart][j]};
            // gmp_printf("%2d-%2d:%Zd\n", i, j, temp.get_mpz_t());
            // triplesMul.getPlain(temp, to_string(i) + "-" + to_string(j));
            this->layer1.X[i].change(j, 0, temp);
        }
        indexStart++;
    }
    mpz_class temp{this->data[indexStart][0]};
    this->output_train.change(0, 0, temp);
    // gmp_printf("temp:%Zd\n", temp.get_mpz_t());
    // triplesMul.getPlain(temp, "ans");
}

//训练
void Lstm::train(string dirData, string dirTriples)
{
    int userStartFLAG = 0;
    this->dirTriples = dirTriples;
    vector<string> files;
    if (!loadDir(dirData, files))
    {
        return;
    }
    int dataSetCounts = files.size();
    for (int dataSetIndex = 0; dataSetIndex < dataSetCounts; dataSetIndex++)
    {
        if (!this->dataCk(files[dataSetIndex]))
        {
            continue;
        }

        int userStart = 0;
        char opt[] = "w";
        if (!userStartFLAG)
        {
            userStart = userStartIndex;
            userStartFLAG++;
            opt[0] = 'a';
        }
        string roleStr = (role == SERVER) ? "SERVER" : "CLIENT";
        string fileName = "./output/logs/" + roleStr + "_ValueTrain_" + to_string(dataSetIndex + 1) + ".csv";
        string fileNameTime = "./output/logs/" + roleStr + "_TimeTrain_" + to_string(dataSetIndex + 1) + ".csv";
        string fileNameTest = "./output/logs/" + roleStr + "_ValueForecast_" + to_string(dataSetIndex + 1) + ".csv";
        string fileNameTimeTest = "./output/logs/" + roleStr + "_TimeForecast_" + to_string(dataSetIndex + 1) + ".csv";
        FILE *fptrTrain = fopen(fileName.c_str(), (const char *)opt);
        FILE *fptrTimeTrain = fopen(fileNameTime.c_str(), (const char *)opt);
        FILE *fptrForecast = fopen(fileNameTest.c_str(), (const char *)opt);
        FILE *fptrTimeForecast = fopen(fileNameTimeTest.c_str(), (const char *)opt);

        int userCounts = this->counts / 480;
        int trainingRounds = (432 - 21) / steps + 1;
        int testingRounds = (48 - 21) / steps + 1;
        cout << "\nUser Counts: " << userCounts
             << " | Training Counts: " << trainingRounds
             << " | Testing Counts: " << testingRounds << endl;
        // showTime("Preparation");
        for (int userIndex = 0; userIndex < userCounts; userIndex++)
        {
            this->dataReadIn(dataSetIndex, userIndex);
            fprintf(fptrTrain, "\nUser No.%d\n\n", userIndex + 1);
            fprintf(fptrTimeTrain, "\nUser No.%d\n\n", userIndex + 1);
            for (int trainingIndex = 0; trainingIndex < trainingRounds; trainingIndex++)
            {
                cout << "\nData Set(" << dataSetIndex + 1 << "|" << dataSetCounts << ")"
                     << " | User(" << userIndex + 1 << "|" << userCounts << ")"
                     << " | Training(" << trainingIndex + 1 << "|" << trainingRounds << ") Begin" << endl;
                this->tripesInit(tripesFLAG, prefix[0]);
                this->dataFill(trainingIndex, TRAIN);
                lastPoint = time(nullptr);
                this->forwardNetwork();
                int roundTimeForword = showTime("TimeTrainForward");
                this->errorDisplay();
                gmp_fprintf(fptrTrain, "%.8Ff,%.8Ff,%.8Ff\n",
                            this->predictiveValue.get_mpf_t(),
                            this->actualValue.get_mpf_t(),
                            this->deltaValue.get_mpf_t());
                fflush(fptrTrain);
                if (trainingIndex == 200)
                    learningRate = learningRate2;
                lastPoint = time(nullptr);
                this->backwardNetwork();
                int roundTimeBackword = showTime("TimeTrainBackward");
                int netTime = showNetTime("TimeTrainNet");

                fprintf(fptrTimeTrain, "%d,%d,%d\n",
                        roundTimeForword,
                        roundTimeBackword,
                        netTime);
                fflush(fptrTimeTrain);
                cout << "Data Set(" << dataSetIndex + 1 << "|5)"
                     << " | User(" << userIndex + 1 << "|" << userCounts << ")"
                     << " | Training(" << trainingIndex + 1 << "|" << trainingRounds << ") Done" << endl;
            }
            cout << "\nData Set No." << dataSetIndex + 1
                 << " | User No." << userIndex + 1
                 << " | Training finished." << endl;
            this->test(userIndex, fptrForecast, fptrTimeForecast);
        }
        fclose(fptrTrain);
        fclose(fptrTimeTrain);
        fclose(fptrForecast);
        fclose(fptrTimeForecast);
    }
}

//测试
void Lstm::test(int userIndex, FILE *fptr, FILE *fptrT)
{
    fprintf(fptr, "\nUser No.%d\n\n", userIndex + 1);
    fprintf(fptrT, "\nUser No.%d\n\n", userIndex + 1);
    int testingRounds = (48 - 21) / steps + 1;
    cout << "Forecast rounds: " << testingRounds << endl;
    for (int testingIndex = 0; testingIndex < testingRounds; testingIndex++)
    {
        cout << "\nForecast No." << testingIndex + 1 << " Begin" << endl;
        this->tripesInit(tripesFLAG, prefix[1]);
        this->dataFill(testingIndex, TEST);
        lastPoint = time(nullptr);
        this->forwardNetwork();
        int roundTimeForword = showTime("TimeForecastForward");
        int netTime = showNetTime("TimeForecastNet");
        fprintf(fptrT, "%d,%d\n",
                roundTimeForword,
                netTime);
        fflush(fptrT);
        this->errorDisplay();
        gmp_fprintf(fptr, "%.8Ff,%.8Ff,%.8Ff\n",
                    this->predictiveValue.get_mpf_t(),
                    this->actualValue.get_mpf_t(),
                    this->deltaValue.get_mpf_t());
        fflush(fptr);
        cout << "Forecast No." << testingIndex + 1 << " Done" << endl;
    }
    cout << "\nUser No." << userIndex + 1 << " Forecast finish." << endl;
}

//读入三元组
void Lstm::tripesInit(int flag, string prefixString)
{
    triplesMul.init(flag, prefixString, this->dirTriples);
}

void Lstm::errorDisplay()
{
    this->predictiveValue = triplesMul.getPlain(this->output.matrix[0][0], "Forecasted value");
    this->actualValue = triplesMul.getPlain(this->output_train.matrix[0][0], "Real value");
    mpf_sub(this->deltaValue.get_mpf_t(), this->predictiveValue.get_mpf_t(), this->actualValue.get_mpf_t());
    gmp_printf("Diff: %.8Ff\n", this->deltaValue.get_mpf_t());
}

//向前计算
void Lstm::forwardNetwork()
{
    cout << "forward begin " << flush;
    this->layer1.forward();
    this->layer2.forward(layer1);
    this->layer3.forward(layer2);
    this->layer4.forward(layer3);
    lstmTools.mCopy(this->layer4.A4, this->output);
    cout << "\rforward finish " << endl;
    // showTime();
}

//向后计算
void Lstm::backwardNetwork()
{
    cout << "backward begin " << flush;
    lstmTools.mCopy(this->output_train, this->layer4.e_t);
    this->layer4.backward(layer3);
    this->layer3.backward(layer4, layer2);
    this->layer2.backward(layer3, layer1);
    this->layer1.backward(layer2);
    cout << "\rbackward finish " << endl;
    // showTime();
}

void Lstm_layer1::forward()
{
    for (int round = 0; round < 20; round++)
    {
        cout << symbol[round % 4] << "\b" << flush;
        Matrix temp40t1_1, temp40t1_2, temp40t1_3;
        Matrix temp_Sj;
        //求f
        // triplesMul.getPlain(this->X[round], "X");
        // triplesMul.getPlain(this->Wfx1, "Wfx1");
        triplesMul.mMul(this->Wfx1, this->X[round], temp40t1_1);
        // triplesMul.getPlain(temp40t1_1, "X * Wfx1");
        if (round)
            triplesMul.mMul(this->Wfh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wfh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bf1, this->block.f);
        triplesMul.sigmoid(this->block.f);
        lstmTools.mCopy(this->block.f, this->F[round]); //保存
        // triplesMul.getPlain(this->block.f, "f");
        //求i
        triplesMul.mMul(this->Wix1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wih1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wih1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bi1, this->block.i);
        triplesMul.sigmoid(this->block.i);
        lstmTools.mCopy(this->block.i, this->I[round]); //保存
        // triplesMul.getPlain(this->block.i, "i");
        //求g
        triplesMul.mMul(this->Wgx1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wgh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wgh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bg1, this->block.g);
        triplesMul.tanh(this->block.g, this->block.g);
        lstmTools.mCopy(this->block.g, this->G[round]); //保存
        // triplesMul.getPlain(this->block.g, "g");
        //求o
        triplesMul.mMul(this->Wox1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Woh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Woh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bo1, this->block.o);
        triplesMul.sigmoid(this->block.o);
        lstmTools.mCopy(this->block.o, this->O[round]); //保存
        // triplesMul.getPlain(this->block.o, "o");
        //求s
        triplesMul.mMull(this->block.g, this->block.i, temp40t1_1);
        if (round)
            triplesMul.mMull(this->S[round - 1], this->block.f, temp40t1_2);
        else
            triplesMul.mMull(this->s_minus1, this->block.f, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, this->S[round]);
        // triplesMul.getPlain(this->S[round], "s");
        //求h
        triplesMul.tanh(this->S[round], temp_Sj);
        triplesMul.mMull(temp_Sj, this->block.o, this->H[round]);
        // triplesMul.getPlain(this->H[round], "h");
    }
}

void Lstm_layer2::forward(Lstm_layer1 layer1)
{
    for (int round = 0; round < 20; round++)
    {
        cout << symbol[round % 4] << "\b" << flush;
        Matrix temp40t1_1, temp40t1_2, temp40t1_3;
        Matrix temp_Sj;
        //求f
        // cout << symbol[0] << "\b" << flush;
        triplesMul.mMul(this->Wfx2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wfh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wfh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bf2, this->block.f);
        triplesMul.sigmoid(this->block.f);
        lstmTools.mCopy(this->block.f, this->F[round]); //保存

        //求i
        // cout << symbol[1] << "\b" << flush;
        triplesMul.mMul(this->Wix2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wih2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wih2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bi2, this->block.i);
        triplesMul.sigmoid(this->block.i);
        lstmTools.mCopy(this->block.i, this->I[round]); //保存

        //求g
        // cout << symbol[2] << "\b" << flush;
        triplesMul.mMul(this->Wgx2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wgh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wgh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bg2, this->block.g);
        triplesMul.tanh(this->block.g, this->block.g);
        lstmTools.mCopy(this->block.g, this->G[round]); //保存

        //求o
        // cout << symbol[3] << "\b" << flush;
        triplesMul.mMul(this->Wox2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Woh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Woh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bo2, this->block.o);
        triplesMul.sigmoid(this->block.o);
        lstmTools.mCopy(this->block.o, this->O[round]); //保存

        //求s
        // cout << symbol[0] << "\b" << flush;
        triplesMul.mMull(this->block.g, this->block.i, temp40t1_1);
        if (round)
            triplesMul.mMull(this->S[round - 1], this->block.f, temp40t1_2);
        else
            triplesMul.mMull(this->s_minus1, this->block.f, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, this->S[round]);

        //求h
        // cout << symbol[1] << "\b" << flush;
        triplesMul.tanh(this->S[round], temp_Sj);
        triplesMul.mMull(temp_Sj, this->block.o, this->H[round]);
        // cout << symbol[3] << "\b" << flush;
    }
    lstmTools.mCopy(this->H[19], this->Z2);
    lstmTools.mCopy(this->Z2, this->A2);
}

void Lstm_layer3::forward(Lstm_layer2 layer2)
{
    cout << "\b  " << flush;
    Matrix temp20t1_1;
    triplesMul.mMul(this->W3, layer2.H[19], temp20t1_1);
    lstmTools.mAdd(temp20t1_1, this->B3, this->Z3);
    triplesMul.tanh(this->Z3, this->A3);
}

void Lstm_layer4::forward(Lstm_layer3 layer3)
{
    Matrix temp1t1_1;
    triplesMul.mMul(this->W4, layer3.A3, temp1t1_1);
    lstmTools.mAdd(temp1t1_1, this->B4, this->Z4);
    triplesMul.tanh(this->Z4, this->A4); // A4=Et`
}

void Lstm_layer4::backward(Lstm_layer3 layer3)
{
    Matrix temp1t1_1, temp1t1_2, temp1t1_3;
    Matrix delta_w4, A3_trans;
    Matrix temp1t20_1, temp1t20_2;
    Matrix const_one{M_CIPHER, 1, 1, 1};
    lstmTools.mSub(this->A4, this->e_t, temp1t1_1);
    // lstmTools.mSub(this->e_t, this->A4, temp1t1_1);
    // triplesMul.getPlain(this->e_t,"真实值");
    // triplesMul.getPlain(this->A4,"预测值");
    //  cout << endl;
    // triplesMul.getPlain(temp1t1_1,"e_t-et");
    // triplesMul.getPlain(this->Z4,"Z4");
    triplesMul.tanh(this->Z4, this->Z4);
    // triplesMul.getPlain(this->Z4,"tanh(Z4)");
    triplesMul.mPoww(this->Z4, temp1t1_2);
    // triplesMul.getPlain(temp1t1_2,"tanh(Z4)^2");
    lstmTools.mSub(const_one, temp1t1_2, temp1t1_3);
    // triplesMul.getPlain(temp1t1_3,"G4");
    triplesMul.mMul(temp1t1_1, temp1t1_3, this->delta_b4); //Δb4
    // triplesMul.getPlain(this->delta_b4,"Δb4");
    lstmTools.mTrans(layer3.A3, A3_trans);
    // triplesMul.getPlain(A3_trans,"A3_trans");
    triplesMul.mMul(this->delta_b4, A3_trans, delta_w4); //ΔW4
    // triplesMul.getPlain(delta_w4,"ΔW4");
    triplesMul.mConstMul(delta_w4, temp1t20_1, learningRate.get_mpz_t());
    // triplesMul.getPlain(temp1t20_1,"ΔW4*0.25");
    triplesMul.mConstMul(this->delta_b4, temp1t1_1, learningRate.get_mpz_t());
    // triplesMul.getPlain(temp1t1_1,"Δb4*0.25");
    lstmTools.mSub(this->W4, temp1t20_1, temp1t20_2);
    lstmTools.mSub(this->B4, temp1t1_1, temp1t1_2);
    // triplesMul.getPlain(this->W4,"old W4");
    // triplesMul.getPlain(this->B4,"old b4");
    lstmTools.mCopy(temp1t20_2, this->W4);
    lstmTools.mCopy(temp1t1_2, this->B4);
    // triplesMul.getPlain(this->W4,"new W4");
    // triplesMul.getPlain(this->B4,"new b4");
}

void Lstm_layer3::backward(Lstm_layer4 layer4, Lstm_layer2 layer2)
{
    Matrix temp1t20_1, temp1t20_2, temp20t1_1, temp20t1_2, temp1t1_1, temp20t40_1, temp20t40_2;
    Matrix G3, W4_trans, A2_trans, delta_W3;
    Matrix const_one{M_CIPHER, 1, 20, 1};
    // triplesMul.getPlain(this->Z3,"Z3");
    triplesMul.tanh(this->Z3, this->Z3);
    // triplesMul.getPlain(this->Z3,"tanh(Z3)");
    triplesMul.mPoww(this->Z3, temp1t20_1);
    // triplesMul.getPlain(temp1t20_1,"tanh(Z3)^2");
    lstmTools.mSub(const_one, temp1t20_1, temp1t20_2);
    // triplesMul.getPlain(temp1t20_2,"1-tanh(Z3)^2");
    lstmTools.mVector2Matrix(temp1t20_2, G3);
    // triplesMul.getPlain(G3,"G3");
    lstmTools.mTrans(layer4.W4, W4_trans);
    // triplesMul.getPlain(W4_trans,"W4_trans");
    triplesMul.mMul(G3, W4_trans, temp20t1_1);
    // triplesMul.getPlain(temp20t1_1,"G3*W4_trans");
    // triplesMul.getPlain(layer4.delta_b4,"delta_b4");
    triplesMul.mMul(temp20t1_1, layer4.delta_b4, this->delta_b3); //Δb3
    // triplesMul.getPlain(this->delta_b3,"delta_b3");
    lstmTools.mTrans(layer2.A2, A2_trans);
    // triplesMul.getPlain(A2_trans,"A2_trans");
    triplesMul.mMul(this->delta_b3, A2_trans, delta_W3); //ΔW3
    // triplesMul.getPlain(delta_W3,"delta_W3");
    triplesMul.mConstMul(delta_W3, temp20t40_1, learningRate.get_mpz_t());
    triplesMul.mConstMul(this->delta_b3, temp20t1_1, learningRate.get_mpz_t());
    // triplesMul.getPlain(this->W3,"old W3");
    // triplesMul.getPlain(this->B3,"old b3");
    // triplesMul.getPlain(temp20t40_1,"delta_W3*learningRate");
    // triplesMul.getPlain(temp20t1_1,"delta_b3*learningRate");
    lstmTools.mSub(this->W3, temp20t40_1, temp20t40_2);
    lstmTools.mSub(this->B3, temp20t1_1, temp20t1_2);
    lstmTools.mCopy(temp20t40_2, this->W3); // W3
    lstmTools.mCopy(temp20t1_2, this->B3);  // b3
    // triplesMul.getPlain(this->W3,"new W3");
    // triplesMul.getPlain(this->B3,"new b3");
}

void Lstm_layer2::backward(Lstm_layer3 layer3, Lstm_layer1 layer1)
{
    Matrix G2{M_DIAGONAL, 1, 40, 40}, const_one{M_CIPHER, 1, 40, 1}, W3_trans;
    Matrix temp40t20_01, temp40t1_01, temp40t1_02, temp40t1_03;
    Matrix Ht, St, Ft, It, Gt, Ot;
    Matrix delta_bf{M_NORMAL, 0, 40, 1}, delta_bi{M_NORMAL, 0, 40, 1};
    Matrix delta_bg{M_NORMAL, 0, 40, 1}, delta_bo{M_NORMAL, 0, 40, 1};
    Matrix delta_Wfx2{M_NORMAL, 0, 40, 40}, delta_Wix2{M_NORMAL, 0, 40, 40};
    Matrix delta_Wgx2{M_NORMAL, 0, 40, 40}, delta_Wox2{M_NORMAL, 0, 40, 40};
    Matrix delta_Wfh2{M_NORMAL, 0, 40, 40}, delta_Wih2{M_NORMAL, 0, 40, 40};
    Matrix delta_Wgh2{M_NORMAL, 0, 40, 40}, delta_Woh2{M_NORMAL, 0, 40, 40};
    lstmTools.mTrans(layer3.W3, W3_trans);
    lstmTools.mLocalMul(G2, W3_trans, temp40t20_01);
    // triplesMul.mMul(G2, W3_trans, temp40t20_01);
    // G2.print("G2(40,40)");
    // triplesMul.getPlain(W3_trans,"W3_trans(40,20)");
    // triplesMul.getPlain(temp40t20_01,"G2*W3_trans(40,20)");
    // triplesMul.getPlain(layer3.delta_b3,"delta_b3(20,1)");
    triplesMul.mMul(temp40t20_01, layer3.delta_b3, Ht); // H
    // triplesMul.getPlain(Ht,"Ht20(40,1)");
    triplesMul.mMull(Ht, this->O[19], temp40t1_01);
    triplesMul.tanh(this->S[19], this->S[19]);
    triplesMul.mPoww(this->S[19], temp40t1_02);
    lstmTools.mSub(const_one, temp40t1_02, temp40t1_03);
    triplesMul.mMull(temp40t1_01, temp40t1_03, St); // S
    // triplesMul.getPlain(St,"St20");

    for (int t = 19; t >= 0;)
    {
        cout << symbol[t % 4] << "\b" << flush;
        Matrix temp40t20_1, temp40t1_1, temp40t1_2, temp40t1_3;
        Matrix Wfh2_trans, Wih2_trans, Wgh2_trans, Woh2_trans;
        Matrix Wfx2_trans, Wix2_trans, Wgx2_trans, Wox2_trans;
        Matrix x_trans, h_trans;
        Matrix delta_bft, delta_bit, delta_bgt, delta_bot;
        Matrix delta_Wfx2t, delta_Wix2t, delta_Wgx2t, delta_Wox2t;
        Matrix delta_Wfh2t, delta_Wih2t, delta_Wgh2t, delta_Woh2t;
        if (t)
            triplesMul.mMull(St, this->S[t - 1], Ft);
        else
            triplesMul.mMull(St, this->s_minus1, Ft);
        triplesMul.mMull(St, this->G[t], It);
        triplesMul.mMull(St, this->I[t], Gt);
        triplesMul.mMull(Ht, this->S[t], Ot); // this->S[t]已经被tanh函数处理过

        lstmTools.mSub(const_one, this->F[t], temp40t1_1);
        triplesMul.mMull(this->F[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ft, temp40t1_2, delta_bft);   //Δbf2t
        lstmTools.mCopy(delta_bft, this->delta_Bf[t]); //保存
        lstmTools.mSub(const_one, this->I[t], temp40t1_1);
        triplesMul.mMull(this->I[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(It, temp40t1_2, delta_bit);   //Δbi2t
        lstmTools.mCopy(delta_bft, this->delta_Bi[t]); //保存
        triplesMul.mPoww(this->G[t], temp40t1_1);
        lstmTools.mSub(const_one, temp40t1_1, temp40t1_2);
        triplesMul.mMull(Gt, temp40t1_2, delta_bgt);   //Δbg2t
        lstmTools.mCopy(delta_bft, this->delta_Bg[t]); //保存
        lstmTools.mSub(const_one, this->O[t], temp40t1_1);
        triplesMul.mMull(this->O[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ot, temp40t1_2, delta_bot);   //Δbo2t
        lstmTools.mCopy(delta_bft, this->delta_Bo[t]); //保存

        lstmTools.mTrans(layer1.H[t], x_trans);
        if (t)
            lstmTools.mTrans(this->H[t - 1], h_trans);
        else
            lstmTools.mTrans(this->h_minus1, h_trans);
        triplesMul.mMul(delta_bft, x_trans, delta_Wfx2t); //ΔWfx2t
        triplesMul.mMul(delta_bft, h_trans, delta_Wfh2t); //ΔWfh2t
        triplesMul.mMul(delta_bit, x_trans, delta_Wix2t); //ΔWix2t
        triplesMul.mMul(delta_bit, h_trans, delta_Wih2t); //ΔWih2t
        triplesMul.mMul(delta_bgt, x_trans, delta_Wgx2t); //ΔWgx2t
        triplesMul.mMul(delta_bgt, h_trans, delta_Wgh2t); //ΔWgh2t
        triplesMul.mMul(delta_bot, x_trans, delta_Wox2t); //ΔWox2t
        triplesMul.mMul(delta_bot, h_trans, delta_Woh2t); //ΔWoh2t

        lstmTools.mAccu(delta_bf, delta_bft);
        lstmTools.mAccu(delta_bi, delta_bit);
        lstmTools.mAccu(delta_bg, delta_bgt);
        lstmTools.mAccu(delta_bo, delta_bot);

        lstmTools.mAccu(delta_Wfx2, delta_Wfx2t);
        lstmTools.mAccu(delta_Wfh2, delta_Wfh2t);
        lstmTools.mAccu(delta_Wix2, delta_Wix2t);
        lstmTools.mAccu(delta_Wih2, delta_Wih2t);
        lstmTools.mAccu(delta_Wgx2, delta_Wgx2t);
        lstmTools.mAccu(delta_Wgh2, delta_Wgh2t);
        lstmTools.mAccu(delta_Wox2, delta_Wox2t);
        lstmTools.mAccu(delta_Woh2, delta_Woh2t);

        if (t == 19)
        {
            Matrix accu_x20{M_NORMAL, 0, 40, 1};
            lstmTools.mTrans(this->Wfx2, Wfx2_trans);
            triplesMul.mMul(Wfx2_trans, delta_bft, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wix2, Wix2_trans);
            triplesMul.mMul(Wix2_trans, delta_bit, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wgx2, Wgx2_trans);
            triplesMul.mMul(Wgx2_trans, delta_bgt, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wox2, Wox2_trans);
            triplesMul.mMul(Wox2_trans, delta_bot, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mCopy(accu_x20, this->X20);
        }
        if (t)
            t--;
        else
            break;
        Matrix accu_h{M_NORMAL, 0, 40, 1}, accu_s{M_NORMAL, 0, 40, 1};
        lstmTools.mTrans(this->Wfh2, Wfh2_trans);
        triplesMul.mMul(Wfh2_trans, delta_bft, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wih2, Wih2_trans);
        triplesMul.mMul(Wih2_trans, delta_bit, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wgh2, Wgh2_trans);
        triplesMul.mMul(Wgh2_trans, delta_bgt, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Woh2, Woh2_trans);
        triplesMul.mMul(Woh2_trans, delta_bot, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mCopy(accu_h, Ht); // h//?
        // triplesMul.getPlain(Ht,"Ht"+to_string(t+1));

        triplesMul.mMull(Ht, this->O[t], temp40t1_1);
        triplesMul.tanh(this->S[t], this->S[t]);
        triplesMul.mPoww(this->S[t], temp40t1_2);
        lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
        triplesMul.mMull(temp40t1_1, temp40t1_3, temp40t1_2);
        triplesMul.mMull(St, this->F[t + 1], temp40t1_1);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, St); // s
        // triplesMul.getPlain(St,"St"+to_string(t+1));
    }
    Matrix temp_W, temp_S;
    triplesMul.mConstMul(delta_Wfx2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wfx2, temp_W);
    triplesMul.mConstMul(delta_Wfh2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wfh2, temp_W);
    triplesMul.mConstMul(delta_Wix2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wix2, temp_W);
    triplesMul.mConstMul(delta_Wih2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wih2, temp_W);
    triplesMul.mConstMul(delta_Wgx2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wgx2, temp_W);
    triplesMul.mConstMul(delta_Wgh2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wgh2, temp_W);
    triplesMul.mConstMul(delta_Wox2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wox2, temp_W);
    triplesMul.mConstMul(delta_Woh2, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Woh2, temp_W);

    triplesMul.mConstMul(delta_bf, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bf2, temp_S);
    triplesMul.mConstMul(delta_bi, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bi2, temp_S);
    triplesMul.mConstMul(delta_bg, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bg2, temp_S);
    triplesMul.mConstMul(delta_bo, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bo2, temp_S);
}

void Lstm_layer1::backward(Lstm_layer2 layer2)
{
    Matrix const_one{M_CIPHER, 1, 40, 1}, W3_trans;
    Matrix temp40t20_1, temp40t1_1, temp40t1_2, temp40t1_3;
    Matrix Ht, St, Ft, It, Gt, Ot, x_trans, h_trans;
    Matrix Wfx2_trans, Wix2_trans, Wgx2_trans, Wox2_trans;
    Matrix Wfh1_trans, Wih1_trans, Wgh1_trans, Woh1_trans;
    Matrix Wfx1_trans, Wix1_trans, Wgx1_trans, Wox1_trans;
    Matrix delta_bft, delta_bit, delta_bgt, delta_bot;
    Matrix delta_Wfx1t, delta_Wix1t, delta_Wgx1t, delta_Wox1t;
    Matrix delta_Wfh1t, delta_Wih1t, delta_Wgh1t, delta_Woh1t;
    Matrix delta_bf{M_NORMAL, 0, 40, 1}, delta_bi{M_NORMAL, 0, 40, 1};
    Matrix delta_bg{M_NORMAL, 0, 40, 1}, delta_bo{M_NORMAL, 0, 40, 1};
    Matrix delta_Wfx1{M_NORMAL, 0, 40, 58}, delta_Wix1{M_NORMAL, 0, 40, 58};
    Matrix delta_Wgx1{M_NORMAL, 0, 40, 58}, delta_Wox1{M_NORMAL, 0, 40, 58};
    Matrix delta_Wfh1{M_NORMAL, 0, 40, 40}, delta_Wih1{M_NORMAL, 0, 40, 40};
    Matrix delta_Wgh1{M_NORMAL, 0, 40, 40}, delta_Woh1{M_NORMAL, 0, 40, 40};

    lstmTools.mCopy(layer2.X20, Ht); // H
    triplesMul.mMull(Ht, this->O[19], temp40t1_1);
    triplesMul.tanh(this->S[19], this->S[19]);
    triplesMul.mPoww(this->S[19], temp40t1_2);
    lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
    triplesMul.mMull(temp40t1_1, temp40t1_3, St); // S
    for (int t = 19; t >= 0;)
    {
        cout << symbol[t % 4] << "\b" << flush;
        if (t)
            triplesMul.mMull(St, this->S[t - 1], Ft);
        else
            triplesMul.mMull(St, this->s_minus1, Ft);
        triplesMul.mMull(St, this->G[t], It);
        triplesMul.mMull(St, this->I[t], Gt);
        triplesMul.mMull(Ht, this->S[t], Ot); // this->S[t]已经被tanh函数处理过

        lstmTools.mSub(const_one, this->F[t], temp40t1_1);
        triplesMul.mMull(this->F[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ft, temp40t1_2, delta_bft); //Δbf2t
        lstmTools.mSub(const_one, this->I[t], temp40t1_1);
        triplesMul.mMull(this->I[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(It, temp40t1_2, delta_bit); //Δbi2t
        triplesMul.mPoww(this->G[t], temp40t1_1);
        lstmTools.mSub(const_one, temp40t1_1, temp40t1_2);
        triplesMul.mMull(Gt, temp40t1_2, delta_bgt); //Δbg2t
        lstmTools.mSub(const_one, this->O[t], temp40t1_1);
        triplesMul.mMull(this->O[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ot, temp40t1_2, delta_bot); //Δbo2t

        lstmTools.mTrans(this->X[t], x_trans);
        if (t)
            lstmTools.mTrans(this->H[t - 1], h_trans);
        else
            lstmTools.mTrans(this->h_minus1, h_trans);
        triplesMul.mMul(delta_bft, x_trans, delta_Wfx1t); //ΔWfx2t
        triplesMul.mMul(delta_bft, h_trans, delta_Wfh1t); //ΔWfh2t
        triplesMul.mMul(delta_bit, x_trans, delta_Wix1t); //ΔWix2t
        triplesMul.mMul(delta_bit, h_trans, delta_Wih1t); //ΔWih2t
        triplesMul.mMul(delta_bgt, x_trans, delta_Wgx1t); //ΔWgx2t
        triplesMul.mMul(delta_bgt, h_trans, delta_Wgh1t); //ΔWgh2t
        triplesMul.mMul(delta_bot, x_trans, delta_Wox1t); //ΔWox2t
        triplesMul.mMul(delta_bot, h_trans, delta_Woh1t); //ΔWoh2t

        lstmTools.mAccu(delta_bf, delta_bft);
        lstmTools.mAccu(delta_bi, delta_bit);
        lstmTools.mAccu(delta_bg, delta_bgt);
        lstmTools.mAccu(delta_bo, delta_bot);

        lstmTools.mAccu(delta_Wfx1, delta_Wfx1t);
        lstmTools.mAccu(delta_Wfh1, delta_Wfh1t);
        lstmTools.mAccu(delta_Wix1, delta_Wix1t);
        lstmTools.mAccu(delta_Wih1, delta_Wih1t);
        lstmTools.mAccu(delta_Wgx1, delta_Wgx1t);
        lstmTools.mAccu(delta_Wgh1, delta_Wgh1t);
        lstmTools.mAccu(delta_Wox1, delta_Wox1t);
        lstmTools.mAccu(delta_Woh1, delta_Woh1t);

        if (t)
            t--;
        else
            break;
        Matrix accu_h{M_NORMAL, 0, 40, 1}, accu_s{M_NORMAL, 0, 40, 1};
        lstmTools.mTrans(this->Wfh1, Wfh1_trans);
        triplesMul.mMul(Wfh1_trans, delta_bft, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wih1, Wih1_trans);
        triplesMul.mMul(Wih1_trans, delta_bit, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wgh1, Wgh1_trans);
        triplesMul.mMul(Wgh1_trans, delta_bgt, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Woh1, Woh1_trans);
        triplesMul.mMul(Woh1_trans, delta_bot, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);

        lstmTools.mTrans(layer2.Wfx2, Wfx2_trans);
        triplesMul.mMul(Wfx2_trans, layer2.delta_Bf[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wix2, Wix2_trans);
        triplesMul.mMul(Wix2_trans, layer2.delta_Bi[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wgx2, Wgx2_trans);
        triplesMul.mMul(Wgx2_trans, layer2.delta_Bg[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wox2, Wox2_trans);
        triplesMul.mMul(Wox2_trans, layer2.delta_Bo[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);

        lstmTools.mCopy(accu_h, Ht);

        triplesMul.mMull(Ht, this->O[t], temp40t1_1);
        triplesMul.tanh(this->S[t], this->S[t]);
        triplesMul.mPoww(this->S[t], temp40t1_2);
        lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
        triplesMul.mMull(temp40t1_1, temp40t1_3, temp40t1_2);
        triplesMul.mMull(St, this->F[t + 1], temp40t1_1);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, St);
    }
    Matrix temp_W, temp_S;
    triplesMul.mConstMul(delta_Wfx1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wfx1, temp_W);
    triplesMul.mConstMul(delta_Wfh1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wfh1, temp_W);
    triplesMul.mConstMul(delta_Wix1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wix1, temp_W);
    triplesMul.mConstMul(delta_Wih1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wih1, temp_W);
    triplesMul.mConstMul(delta_Wgx1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wgx1, temp_W);
    triplesMul.mConstMul(delta_Wgh1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wgh1, temp_W);
    triplesMul.mConstMul(delta_Wox1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Wox1, temp_W);
    triplesMul.mConstMul(delta_Woh1, temp_W, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Woh1, temp_W);
    triplesMul.mConstMul(delta_bf, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bf1, temp_S);
    triplesMul.mConstMul(delta_bi, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bi1, temp_S);
    triplesMul.mConstMul(delta_bg, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bg1, temp_S);
    triplesMul.mConstMul(delta_bo, temp_S, learningRate.get_mpz_t());
    lstmTools.mAccuSub(this->Bo1, temp_S);
}

//将W和b导出到文件
void Lstm::saveMatrix(string fileName)
{
    FILE *fp = fopen(fileName.c_str(), "w");
    if (fp != nullptr)
    {
        // layer1
        // Wx
        fprintf(fp, "%s\n", this->layer1.Wfx1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Wix1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Wgx1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Wox1.serialization().c_str());
        fflush(fp);
        // Wh
        fprintf(fp, "%s\n", this->layer1.Wfh1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Wih1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Wgh1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Woh1.serialization().c_str());
        fflush(fp);
        // b
        fprintf(fp, "%s\n", this->layer1.Bf1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Bi1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Bg1.serialization().c_str());
        fprintf(fp, "%s\n", this->layer1.Bo1.serialization().c_str());
        fflush(fp);

        // layer2
        // Wx
        fprintf(fp, "%s\n", this->layer2.Wfx2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Wix2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Wgx2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Wox2.serialization().c_str());
        fflush(fp);
        // Wh
        fprintf(fp, "%s\n", this->layer2.Wfh2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Wih2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Wgh2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Woh2.serialization().c_str());
        fflush(fp);
        // b
        fprintf(fp, "%s\n", this->layer2.Bf2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Bi2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Bg2.serialization().c_str());
        fprintf(fp, "%s\n", this->layer2.Bo2.serialization().c_str());
        fflush(fp);

        // layer3
        // W
        fprintf(fp, "%s\n", this->layer3.W3.serialization().c_str());
        // b
        fprintf(fp, "%s\n", this->layer3.B3.serialization().c_str());
        fflush(fp);

        // layer4
        // W
        fprintf(fp, "%s\n", this->layer4.W4.serialization().c_str());
        // b
        fprintf(fp, "%s\n", this->layer4.B4.serialization().c_str());
        fflush(fp);
    }
    else
        cout << "Save error!" << endl;
    fclose(fp);
}

//将W和b从文件导入
void Lstm::loadMatrix(string fileName)
{
    ifstream infile;
    infile.open(fileName, ios::in);
    if (infile)
    {
        // layer1
        string Wfx1_str, Wix1_str, Wgx1_str, Wox1_str;
        string Wfh1_str, Wih1_str, Wgh1_str, Woh1_str;
        string bf1_str, bi1_str, bg1_str, bo1_str;
        // Wx
        getline(infile, Wfx1_str);
        this->layer1.Wfx1.deserialization(Wfx1_str);
        getline(infile, Wix1_str);
        this->layer1.Wix1.deserialization(Wix1_str);
        getline(infile, Wgx1_str);
        this->layer1.Wgx1.deserialization(Wgx1_str);
        getline(infile, Wox1_str);
        this->layer1.Wox1.deserialization(Wox1_str);
        // Wh
        getline(infile, Wfh1_str);
        this->layer1.Wfh1.deserialization(Wfh1_str);
        getline(infile, Wih1_str);
        this->layer1.Wih1.deserialization(Wih1_str);
        getline(infile, Wgh1_str);
        this->layer1.Wgh1.deserialization(Wgh1_str);
        getline(infile, Woh1_str);
        this->layer1.Woh1.deserialization(Woh1_str);
        // b
        getline(infile, bf1_str);
        this->layer1.Bf1.deserialization(bf1_str);
        getline(infile, bi1_str);
        this->layer1.Bi1.deserialization(bi1_str);
        getline(infile, bg1_str);
        this->layer1.Bg1.deserialization(bg1_str);
        getline(infile, bo1_str);
        this->layer1.Bo1.deserialization(bo1_str);

        // layer2
        string Wfx2_str, Wix2_str, Wgx2_str, Wox2_str;
        string Wfh2_str, Wih2_str, Wgh2_str, Woh2_str;
        string bf2_str, bi2_str, bg2_str, bo2_str;
        // Wx
        getline(infile, Wfx2_str);
        this->layer2.Wfx2.deserialization(Wfx2_str);
        getline(infile, Wix2_str);
        this->layer2.Wix2.deserialization(Wix2_str);
        getline(infile, Wgx2_str);
        this->layer2.Wgx2.deserialization(Wgx2_str);
        getline(infile, Wox2_str);
        this->layer2.Wox2.deserialization(Wox2_str);
        // Wh
        getline(infile, Wfh2_str);
        this->layer2.Wfh2.deserialization(Wfh2_str);
        getline(infile, Wih2_str);
        this->layer2.Wih2.deserialization(Wih2_str);
        getline(infile, Wgh2_str);
        this->layer2.Wgh2.deserialization(Wgh2_str);
        getline(infile, Woh2_str);
        this->layer2.Woh2.deserialization(Woh2_str);
        // b
        getline(infile, bf2_str);
        this->layer2.Bf2.deserialization(bf2_str);
        getline(infile, bi2_str);
        this->layer2.Bi2.deserialization(bi2_str);
        getline(infile, bg2_str);
        this->layer2.Bg2.deserialization(bg2_str);
        getline(infile, bo2_str);
        this->layer2.Bo2.deserialization(bo2_str);

        // layer3
        string W3, b3;
        // W
        getline(infile, W3);
        this->layer3.W3.deserialization(W3);
        // b
        getline(infile, b3);
        this->layer3.B3.deserialization(b3);

        // layer4
        string W4, b4;
        // W
        getline(infile, W4);
        this->layer4.W4.deserialization(W4);
        // b
        getline(infile, b4);
        this->layer4.B4.deserialization(b4);
    }
    else
        cout << "Load error!" << endl;
    infile.close();
}
