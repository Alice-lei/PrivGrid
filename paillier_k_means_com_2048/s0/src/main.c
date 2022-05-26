#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "paillier.h"
#include<math.h>
#include<time.h>
#include<gmp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_PORT 6666
#include "tools.h"
#define Center 5
#define Dim 48
void compute_add(mpz_t** data,mpz_t** add,int point_flag[],int num_point_of_center[],paillier_public_key *pub1,int num_of_point);
void computer_center(int num_point_of_center[],mpz_t** add,mpz_t** center,paillier_public_key *pub1);
void decrypt_distance(mpz_t** distence,paillier_private_key *priv0,int num_of_point);
void classification(mpz_t** distence,int point_flag[],int num_of_point);
void initialization(int num_point_of_center[],mpz_t** add,mpz_t* center_dis,mpz_t pk1_zero_c);
//Server S0 has the encrypted data of public key PK1 of server S1
int main(int argc, char* argv[])
{
    char c;
    int num_of_point = 0, i, j, k, * point_flag,* num_point_of_center;
    clock_t start,finish;
    double duration;
    start=clock();
    FILE * fp_data_pk1,* pk0, * pk1, * sk0,* data_flag;
    paillier_public_key pub0, pub1;
    paillier_private_key priv0;
    paillier_public_init(&pub1);
    paillier_public_init(&pub0);
    paillier_private_init(&priv0);
    mpz_t** data_pk1;//Save data encrypted with PK1
    //Read data from the file and open the file
    fp_data_pk1 = fopen("data_pk1", "r");
    pk1 = fopen("pk1", "r");
    pk0 = fopen("pk0", "r");
    sk0 = fopen("priv0", "r");
    data_flag=fopen("data_flag","w");
    if(!(fp_data_pk1)&&!(pk1)&&!(pk0)&&!(sk0)&&!(data_flag))
    {

        printf("fail to open the file! \n");
        return 0;
    }
    else
    {
        printf("The file was successfully opened\n");
        while ((c = fgetc(fp_data_pk1)) != EOF)//Get the number of rows
        {
            if (c == '\n')
                num_of_point++;
        }
        num_of_point = num_of_point / Dim;//Divide by the dimension to get the number of data points
        printf("Number of data points %d\n", num_of_point);
        data_pk1 = (mpz_t**)malloc(sizeof(mpz_t*) * num_of_point);
        point_flag = (int*)malloc(sizeof(int) * num_of_point);
        for (i = 0; i < num_of_point; i++)//initialization
        {
            data_pk1[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
            for (j = 0; j < Dim; j++)
            {
                mpz_init(data_pk1[i][j]);
            }
        }
        rewind(fp_data_pk1);//Go back to the head of the file and start reading the data
        k = 0;
        for (i = 0; i < num_of_point; i++)
        {
            for (j = 0; j < Dim; j++)
            {
                gmp_fscanf(fp_data_pk1, "%Zx\n", data_pk1[i][j]);
            }
            point_flag[i] = k % Center;//Initial classification
            k++;
        }
    }
    paillier_public_in_str(&pub1, pk1);
    paillier_public_in_str(&pub0, pk0);
    paillier_private_in_str(&priv0, sk0);//Read out the public key sum of the two servers and the private key of server S0 from the file
    printf("All data were read successfully\n");
    fclose(fp_data_pk1);
    fclose(pk0);
    fclose(pk1);
    fclose(sk0);//Close file pointer
    mpz_t** add, pk1_zero_c, ** center_new, zero_m;
    mpz_t** distence, * center_dis;
    mpz_init(pk1_zero_c);
    mpz_init_set_si(zero_m,0);
    paillier_encrypt(pk1_zero_c, zero_m, &pub1);
    center_dis = (mpz_t*)malloc(sizeof(mpz_t) * Center);
    add = (mpz_t**)malloc(sizeof(mpz_t*) * Center);
    center_new = (mpz_t**)malloc(sizeof(mpz_t*) * Center);
    distence = (mpz_t**)malloc(sizeof(mpz_t*) * num_of_point);
    num_point_of_center = (int*)malloc(sizeof(int) * Center);
    memset(num_point_of_center, 0, sizeof(int) * Center);//initialization
    printf("ok\n");
    for (i = 0; i < Center; i++)
    {
        mpz_init_set_si(center_dis[i], 0);
        add[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
        center_new[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
        for (j = 0; j < Dim; j++)
        {
            mpz_init(add[i][j]);
            mpz_init_set_si(center_new[i][j],0);
            mpz_set(add[i][j], pk1_zero_c);//Initialize to ciphertext 0

        }
    }
    for (i = 0; i < num_of_point; i++)
    {
        distence[i] = (mpz_t*)malloc(sizeof(mpz_t) * Center);
        for (j = 0; j < Center; j++)
        {
            mpz_init_set_si(distence[i][j], 0);//Initialize the distance to the module of a centroid encrypted by pk0
        }
    }
    /*-----------About the relevant variables and functions of communication----------------*/
    //File descriptor returned by calling socket function
    int serverSocket;
    //Declare two socket sockaddrs_ In structure variable, representing client and server respectively
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    //Socket function, failure returns - 1
    //int socket(int domain, int type, int protocol);
    //The first parameter indicates the address type used, generally IPv4 and AF_ INET
    //The second parameter indicates the socket type: TCP: connection oriented stable data transmission sock_ STREAM
    //The third parameter is set to 0
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    //Initialize the socket on the server side, and use htons and htonl to convert the port and address into network byte order
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    //IP is the IP of this server, or you can use the macro inaddr_ Any instead, stands for 0.0.0.0, indicating all addresses
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //For functions such as bind and accept, the socket parameters need to be forcibly converted to (struct SOCKADDR *)
    //Bind has three parameters: the file descriptor of the socket on the server side
    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }
    //Set the socket on the server to listen
    if (listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    printf("Listening port: %d\n", SERVER_PORT);
    client = accept(serverSocket, (struct sockaddr *) &clientAddr, (socklen_t *) &addr_len);
    if (client < 0)
    {
        perror("accept");
    }
    printf("Waiting for message...\n");
    //inet_ The ntoa IP address conversion function converts the network byte order IP to dotted decimal IP
    //expression：char *inet_ntoa (struct in_addr);
    printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("Port is %d\n", htons(clientAddr.sin_port));
    /*-----------Related variables and function end of communication----------------*/
    /*---------------------------
    --------The cycle begins-----------
    ---------------------------*/
    int loop=0,flag=0,ret,byte,midbyte;
    char transfer[1024],transfer1[1024];
    mpz_t y;
    mpz_init(y);
    while(flag==0&&loop<100)
    {

        loop++;
        printf("\n\n这是第%d轮循环!\n",loop);
        //S0 has PK1 encrypted data
        //Calculate the sum of various coordinates. S1 has the classification of each point, that is, point_ Flag, also pay attention to the variables to be initialized in the loop
        compute_add(data_pk1,add,point_flag,num_point_of_center,&pub1,num_of_point);
        printf("\n");
        computer_center(num_point_of_center,add,center_new,&pub1);
   
        //In this position, S0 will enter the center_ The centroid can be sent to S1, which is equivalent to new
        printf("Start sending data:center_new\n");
        //gmp_printf("%Zd   \n",center_new[0][0]);
        ret=0;
        for(i=0; i<Center; i++)
        {
            for(j=0; j<Dim; j++)
            {
                memset(transfer,0,sizeof(transfer));
                mpz_get_str(transfer,62,center_new[i][j]);
                ret+=send(client,transfer,sizeof(transfer),0);
            }
        }
        printf("Sending data succeeded    %d\n",ret);
        /*SS1 starts to recalculate the distance from each data point to each centroid Start receiving (distance [i] [J]) pk0 sent by S1 at this location*/
        //First accept the flag at this position. If flag = 1, it means that the center point has remained stable
        printf("Start receiving data flag\n");
        byte=recv(client,&flag,sizeof(flag),0);
        printf("Data flag received successfully:%d   %d\n",flag,byte);
        if(flag==0)
        {
            printf("Start receiving data distance\n");
            byte=0;
            k=0;
            for(i=0; i<num_of_point; i++)
            {
                for(j=0; j<Center; j++)
                {
                    k++;
                    memset(transfer,0,sizeof(transfer));
                    midbyte=recv(client,transfer,sizeof(transfer),0);
                    byte+=midbyte;
                    if(midbyte!=1024)
                    {
                        midbyte=recv(client,transfer1,sizeof(transfer1),0);
                        byte+=midbyte;

                    }
                    mpz_set_str(y,transfer,62);
                    mpz_set(distence[i][j],y);
                }
            }
            printf("Receiving data distance succeeded   %d     %d\n",byte,k);
            /*Send (distance [i] [j]) pk0 to S0
               S0 starts to reclassify all data points, and then you can write a selection sort*/
            decrypt_distance(distence,&priv0,num_of_point);
            classification(distence,point_flag,num_of_point);
            //It is equivalent to getting the flag array. Next, go to S0 and let S0 calculate a new round of centroid for circulation
	    //Each cycle needs to initialize some variables
            initialization(num_point_of_center,add,center_dis,pk1_zero_c);
        }
    }
    close(serverSocket);
    finish=clock();
    printf("Successfully closed communication\n");
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("Running time%.2lf seconds ",duration);
    mpz_clear(pk1_zero_c);
    mpz_clear(zero_m);
    for (i = 0; i < num_of_point; i++)
    {
        free(*(data_pk1 + i));
    }//Free memory
    paillier_public_clear(&pub0);
    paillier_public_clear(&pub1);
    return 0;

}







//Function part
//Calculate the sum of each dimension of all data of each class. The dimension here is 48
void compute_add(mpz_t** data,mpz_t** add,int point_flag[],int num_point_of_center[],paillier_public_key *pub1,int num_of_point)
{
    int i,j;
    for (i = 0; i < num_of_point; i++)
    {
        //printf("%d %d\n",i,point_flag[i]);
        switch (point_flag[i])
        {
        case 0:
            num_point_of_center[0]++;
            //printf("分类0:%d\n",num_point_of_center[0]);
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_add(add[0][j], add[0][j], data[i][j], pub1);
            }
            break;
        case 1:
            num_point_of_center[1]++;
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_add(add[1][j], add[1][j], data[i][j], pub1);
            }
            break;
        case 2:
            num_point_of_center[2]++;
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_add(add[2][j], add[2][j], data[i][j], pub1);
            }
            break;
        case 3:
            num_point_of_center[3]++;
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_add(add[3][j], add[3][j], data[i][j], pub1);
            }
            break;
        case 4:
            num_point_of_center[4]++;
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_add(add[4][j], add[4][j], data[i][j], pub1);
            }
            break;
        default:
            printf("add find error!\n");
            break;
        }
    }
}
//Calculate the center point of each class
void computer_center(int num_point_of_center[],mpz_t** add,mpz_t** center,paillier_public_key *pub1)
{
    int i,j,ti[Center];
    memset(ti, 0, sizeof(int) * Center);
    mpz_t m_ti;
    mpz_init(m_ti);
    for (i = 0; i < Center; i++)
    {
        if(num_point_of_center[i]!=0)//Prevent the number of a class from being 0
        {
            ti[i] = (1.0 / num_point_of_center[i]) * 1000000;
            mpz_set_ui(m_ti, ti[i]);//Converting int type Ti to MPZ_ Type T, ready for homomorphic multiplication
            gmp_printf("%Zd\n",m_ti);
            for (j = 0; j < Dim; j++)
            {
                paillier_homomorphic_multc(center[i][j], add[i][j], m_ti, pub1);
            }
        }
        else
        {
            printf("The number of class %d is 0",i+1);
        }
    }
}
//The server S0 decrypts the distance sent by the server S1
void decrypt_distance(mpz_t** distence,paillier_private_key *priv0,int num_of_point)
{
    int i,j;
    for (i = 0; i < num_of_point; i++)//S0 decrypts the distance
    {
        for (j = 0; j < Center; j++)
        {
            paillier_decrypt(distence[i][j], distence[i][j], priv0);
        }
    }
}
//The server S0 re classifies according to the decrypted distance
void classification(mpz_t** distence,int point_flag[],int num_of_point)
{
    int i,j;
    mpz_t min;
    mpz_init(min);
    for (i = 0; i < num_of_point; i++)//Find a new round of classification
    {
        point_flag[i]=0;
        mpz_set(min,distence[i][0]);
        for (j = 1; j < Center; j++)
        {
            if (mpz_cmp(distence[i][j], min) < 0)
            {
                point_flag[i] = j;
                mpz_set(min, distence[i][j]);
            }
        }
    }
}
void initialization(int num_point_of_center[],mpz_t** add,mpz_t* center_dis,mpz_t pk1_zero_c)
{
    int i,j;
    memset(num_point_of_center, 0, sizeof(int) * Center);//initialization
    for (i = 0; i < Center; i++)
    {
        mpz_set_si(center_dis[i], 0);
        for(j=0; j<Dim; j++)
        {
            mpz_set(add[i][j], pk1_zero_c);
        }
    }
}
