#include "network.h"

using namespace std;

//网络初始化
void Network::init(int port)
{
    cout << "Network preparing " << flush;
    this->port = port;
    //创建socket
    if ((this->sockSer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket error : %s\n", strerror(errno));
        exit(1);
    }
    //填充套接字地址结构，包括地址族，ip和端口号
    bzero(&this->addrSer, sizeof(struct sockaddr_in));
    inet_aton((const char *)IP.c_str(), &(this->addrSer.sin_addr));
    this->addrSer.sin_family = AF_INET;
    this->addrSer.sin_port = htons(port);
    int opt = SO_REUSEADDR;
    setsockopt(this->sockSer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // 接收缓冲区
    int nRecvBuf = 8 * 1024; //设置为8K
    setsockopt(this->sockSer, SOL_SOCKET, SO_RCVBUF, (const char *) &nRecvBuf, sizeof(int));
    // 发送缓冲区
    int nSendBuf = 8 * 1024; //设置为8K
    setsockopt(this->sockSer, SOL_SOCKET, SO_SNDBUF, (const char *) &nSendBuf, sizeof(int));
    if (role == SERVER)
    {
        //绑定
        if (bind(sockSer, (struct sockaddr *) (&this->addrSer), sizeof(struct sockaddr)) == -1)
        {
            printf("Bind error : %s\n", strerror(errno));
            exit(1);
        }
        // 监听
        if (listen(sockSer, 1) == -1)
        {
            printf("Listen error : %s\n", strerror(errno));
            exit(1);
        }
        //接受
        socklen_t naddr = sizeof(struct sockaddr_in);
        if ((this->sockCli = accept(this->sockSer, (struct sockaddr *) (&this->addrCli), &naddr)) == -1)
        {
            printf("Accept error%s\n", strerror(errno));
            exit(1);
        }
    } else
    {
        //连接
        int times = 1;
        while (connect(sockSer, (struct sockaddr *) &this->addrSer, sizeof(struct sockaddr)) == -1)
        {
            cout << symbol[times % 4] << "\b" << flush;
            sleep(times++);
            if (times > 11)
            {
                printf("Connect error : %s\n", strerror(errno));
                exit(1);
            }
        }
    }
    cout << "\rNetwork OK        " << endl;
}

//发送一个string
bool Network::mSend(int fd, string send_string)
{
    char *cstr = new char[send_string.size() + 1];
    strcpy(cstr, send_string.c_str());
    size_t send_size = send_string.size() + 1; //需要发送的数据大小
    string send_size_string = to_string(send_size);
    char *c_send_size_str = new char[send_size_string.size() + 1];
    strcpy(c_send_size_str, send_size_string.c_str());
    if (send(fd, c_send_size_str, send_size_string.size() + 1, 0) == -1)
    { //告知对方需要准备的缓冲区大小
        printf("Prewrite error : %s\n", strerror(errno));
        exit(1);
    }
    if (recv(fd, this->checkBuf, BUF_SIZE, 0) == -1 && !(strcmp(this->checkBuf, c_send_size_str)))
    { //接收确认信息
        printf("Check error : %s\n", strerror(errno));
        exit(1);
    }
    clock_t startS = clock();
    if ((send(fd, cstr, send_size, 0)) == -1)
    { //发送数据
        printf("Write error : %s\n", strerror(errno));
        exit(1);
    }
    clock_t endS = clock();
    double duration = (double) (endS - startS) / CLOCKS_PER_SEC;
    NetDuration += duration * 1000;
    double speed = send_size / 1000000.0 / duration;
    speedS += speed;
    countS++;
    delete[] cstr;
    delete[] c_send_size_str;
    return true;
}

//接收一个string
bool Network::mReceive(int fd, string &recv_string)
{
    recv_string.clear();
    if (recv(fd, this->recvSizeBuf, BUF_SIZE, 0) == -1)
    { //接收缓冲区尺寸
        printf("Preread error : %s\n", strerror(errno));
        exit(1);
    }
    if (send(fd, this->recvSizeBuf, BUF_SIZE, 0) == -1)
    { //发送确认信息
        printf("Check error : %s\n", strerror(errno));
        exit(1);
    }
    size_t recv_size = stol(this->recvSizeBuf); //接收缓冲区尺寸
    char *cstr = new char[recv_size];           //接收缓冲区
    memset(cstr, '\0', recv_size);
    ssize_t recv_num, remain_num = recv_size;
    clock_t startR = clock();
    while (remain_num > 1)
    {
        if ((recv_num = recv(fd, cstr, recv_size, 0)) == -1 || recv_num == 0)
        { //接收数据
            printf("Read error : %s\n", strerror(errno));
            exit(1);
        }
        remain_num -= strlen(cstr);
        recv_string += cstr;
        memset(cstr, '\0', recv_size);
    }
    clock_t endR = clock();
    double duration = (double) (endR - startR) / CLOCKS_PER_SEC;
    NetDuration += duration * 1000;
    double speed = recv_size / 1000000.0 / duration;
    speedR += speed;
    countR++;
    delete[] cstr;
    return true;
}

//发送一个string
bool Network::mSend(string send_string)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    this->mSend(fd, send_string);
    return true;
}

//接收一个string
bool Network::mReceive(string &recv_string)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    this->mReceive(fd, recv_string);
    return true;
}

//发送mpz_class数
bool Network::mSend(mpz_class send)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string send_string;
    this->serialization(send, send_string);
    this->mSend(fd, send_string);
    return true;
}

//发送两个mpz_class数
bool Network::mSend(mpz_class send1, mpz_class send2)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string send_string, send_string1, send_string2;
    this->serialization(send1, send_string1);
    this->serialization(send2, send_string2);
    send_string = send_string1 + mDelim + send_string2;
    this->mSend(fd, send_string);
    return true;
}

//接收mpz_class数
bool Network::mReceive(mpz_class &receive)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string recv_string;
    this->mReceive(fd, recv_string);
    this->deserialization(recv_string, receive);
    return true;
}

//接收两个mpz_class数
bool Network::mReceive(mpz_class &receive1, mpz_class &receive2)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string recv_string;
    this->mReceive(fd, recv_string);
    char *pch1, *pch2;
    char *cstr = stringToChar(recv_string);
    pch1 = strtok(cstr, mDelim);//第一个数
    pch2 = strtok(nullptr, mDelim);//第二个数
    this->deserialization(pch1, receive1);
    this->deserialization(pch2, receive2);
    delete[] cstr;
    return true;
}

//发送矩阵
bool Network::mSend(Matrix matrix)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string send_string, recv_string;
    this->serialization(matrix, send_string);
    this->mSend(fd, send_string);
    this->mReceive(fd, recv_string);
    if (recv_string != to_string(send_string.size()))
    { //若尺寸不对
        cout << "Send matrix error!" << endl;
        cout << "Send size: " << to_string(send_string.size()) << " | Recv size: " << recv_string << endl;
        // exit(1);
    }
    return true;
}

//发送两个矩阵
bool Network::mSend(Matrix matrix1, Matrix matrix2)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    string send_string, send_string1, send_string2, recv_string;
    this->serialization(matrix1, send_string1);
    this->serialization(matrix2, send_string2);
    send_string = send_string1 + xDelim + send_string2;
    this->mSend(fd, send_string);
    this->mReceive(fd, recv_string);
    if (recv_string != to_string(send_string.size()))
    { //若尺寸不对
        cout << "Send matrix error!" << endl;
        cout << "Send size: " << to_string(send_string.size()) << " | Recv size: " << recv_string << endl;
        // exit(1);
    }
    return true;
}

//接收矩阵
bool Network::mReceive(Matrix &matrix)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    Matrix matrix_recv;
    string recv_string;
    this->mReceive(fd, recv_string);                 //接收矩阵
    this->mSend(fd, to_string(recv_string.size()));  //发送接收尺寸
    this->deserialization(recv_string, matrix_recv); //反序列化
    this->networkTools.mCopy(matrix_recv, matrix);
    return true;
}

//接收两个矩阵
bool Network::mReceive(Matrix &matrix1, Matrix &matrix2)
{
    int fd = (role == SERVER) ? this->sockCli : sockSer;
    Matrix matrix_recv1, matrix_recv2;
    string recv_string;
    this->mReceive(fd, recv_string);                 //接收矩阵
    this->mSend(fd, to_string(recv_string.size()));  //发送接收尺寸
    char *pch1, *pch2;
    char *cstr = stringToChar(recv_string);
    pch1 = strtok(cstr, xDelim);//第一个矩阵
    pch2 = strtok(nullptr, xDelim);//第二个矩阵
    this->deserialization(pch1, matrix_recv1); //反序列化
    this->deserialization(pch2, matrix_recv2); //反序列化
    this->networkTools.mCopy(matrix_recv1, matrix1);
    this->networkTools.mCopy(matrix_recv2, matrix2);
    delete[] cstr;
    return true;
}

//发送矩阵数组
bool Network::mSend(array<Matrix, 5> array)
{
    string send_string, recv_string;
    for (int i = 0; i < 5; i++)
        this->mSend(array[i]); //发送矩阵
    return true;
}

//接收矩阵数组
bool Network::mReceive(array<Matrix, 5> &array)
{
    string recv_string;
    for (int i = 0; i < 5; i++)
        this->mReceive(array[i]); //接收矩阵
    return true;
}

//数序列化
bool Network::serialization(mpz_class num, string &num_ser)
{
    num_ser = num.get_str();
    return true;
}

//矩阵序列化
bool Network::serialization(Matrix matrix, string &matrix_ser)
{
    string matrix_string;
    string matrix_string_row = to_string(matrix.row);
    string matrix_string_col = to_string(matrix.col);
    matrix_string = matrix_string_row + mDelim + matrix_string_col + mDelim;
    for (int mRow = 0; mRow < matrix.row; mRow++)
    {
        for (int mCol = 0; mCol < matrix.col; mCol++)
        {
            string temp = matrix.matrix[mRow][mCol].get_str();
            matrix_string += temp;
            if ((mRow < matrix.row - 1) || (mCol < matrix.col - 1))
                matrix_string += mDelim;
        }
    }
    matrix_ser = matrix_string;
    return true;
}

//数反序列化
bool Network::deserialization(string num_ser, mpz_class &num)
{
    num = mpz_class(num_ser, baseNum);
    return true;
}

//矩阵反序列化
bool Network::deserialization(string matrix_ser, Matrix &matrix)
{
    char *pch;
    char *cstr = stringToChar(matrix_ser);
    pch = strtok(cstr, mDelim);
    if (pch == nullptr)
    {
        printf("Reveice matrix error!(1) : %s\n", strerror(errno));
        exit(1);
    }
    int matrix_row = stoi(pch);
    pch = strtok(nullptr, mDelim);
    if (pch == nullptr)
    {
        printf("Reveice matrix error!(2) : %s\n", strerror(errno));
        exit(1);
    }
    int matrix_col = stoi(pch);

    Matrix matrix_temp(M_NORMAL, 0, matrix_row, matrix_col);
    for (int i = 0; i < matrix_row; i++)
    {
        for (int j = 0; j < matrix_col; j++)
        {
            pch = strtok(nullptr, mDelim);
            if (pch == nullptr)
            {
                cout << "Exchange error!(2)" << endl;
                exit(1);
            }
            mpz_class temp = mpz_class(pch, 10);
            matrix_temp.change(i, j, temp);
        }
    }
    this->networkTools.mCopy(matrix_temp, matrix);
    delete[] cstr;
    return true;
}

//关闭套接字
Network::~Network()
{
    if (sockSer)
        close(sockSer);
    if (sockCli)
        close(sockCli);
}