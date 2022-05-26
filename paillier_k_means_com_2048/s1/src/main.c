#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "paillier.h"
#include<math.h>
#include<string.h>
#include<gmp.h>
#include "tools.h"
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
#define Center 5
#define Dim 48
void decrypt_center(mpz_t** center_new,paillier_private_key* priv1);
int cmp_old_new(mpz_t** center_new,mpz_t** center_old);
void copy_new_to_old(mpz_t** center_new,mpz_t** center_old);
void compute_mu(mpz_t* center_dis,mpz_t** center_new);
void encrypt_mu(mpz_t* center_dis,paillier_public_key* pub0);
void initialization_dis(mpz_t** distence,mpz_t* center_dis,int num_of_point);
void printf_center(mpz_t** center);
//Server S1 has the encrypted data of public key PK0 of server S0
int main(int argc, char* argv[])
{
    char c;
    int num_of_point = 0, i, j, k, * point_flag,* num_point_of_center;
    FILE * fp_data_pk0,* pk0, * pk1, * sk1;
    paillier_public_key pub0, pub1;
    paillier_private_key priv1;
    paillier_public_init(&pub1);
    paillier_public_init(&pub0);
    paillier_private_init(&priv1);
    mpz_t** data_pk0;//Save data encrypted with pk0
    //Read data from the file and open the file
    fp_data_pk0 = fopen("data_pk0", "r");
    pk1 = fopen("pk1", "r");
    pk0 = fopen("pk0", "r");
    sk1 = fopen("priv1", "r");
    if(!(fp_data_pk0)&&!(pk1)&&!(pk0)&&!(sk1))
    {
        printf("fail to open the file! \n");
        return 0;
    }
    else
    {
        while ((c = fgetc(fp_data_pk0)) != EOF)//Get the number of rows
        {
            if (c == '\n')
                num_of_point++;
        }
        num_of_point = num_of_point / Dim;//Divide by the dimension to get the number of data points
        printf("数据点的个数 %d\n", num_of_point);
        data_pk0 = (mpz_t**)malloc(sizeof(mpz_t*) * num_of_point);
        point_flag = (int*)malloc(sizeof(int) * num_of_point);
        for (i = 0; i < num_of_point; i++)//initialization
        {
            data_pk0[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
            for (j = 0; j < Dim; j++)
            {
                mpz_init(data_pk0[i][j]);
            }
        }
        rewind(fp_data_pk0);//Go back to the head of the file and start reading the data
        k = 0;
        for (i = 0; i < num_of_point; i++)
        {
            for (j = 0; j < Dim; j++)
            {
                gmp_fscanf(fp_data_pk0, "%Zx\n", data_pk0[i][j]);
            }
            point_flag[i] = k % Center;//Initial classification
            k++;
        }
    }
    paillier_public_in_str(&pub1, pk1);
    paillier_public_in_str(&pub0, pk0);
    paillier_private_in_str(&priv1, sk1);//Read out the public key sum of the two servers and the private key of server S1 from the file
    printf("All data were read successfully\n");
    fclose(fp_data_pk0);
    fclose(pk0);
    fclose(pk1);
    fclose(sk1);//Close file pointer
    mpz_t** add, ** center_new,** center_old,mid,mid1,mid2;
    mpz_t** distence, * center_dis;
    mpz_init(mid);
    mpz_init(mid1);
    mpz_init(mid2);
    center_dis = (mpz_t*)malloc(sizeof(mpz_t) * Center);
    add = (mpz_t**)malloc(sizeof(mpz_t*) * Center);
    center_new = (mpz_t**)malloc(sizeof(mpz_t*) * Center);
    center_old = (mpz_t**)malloc(sizeof(mpz_t*) * Center);
    distence = (mpz_t**)malloc(sizeof(mpz_t*) * num_of_point);
    num_point_of_center = (int*)malloc(sizeof(int) * Center);
    memset(num_point_of_center, 0, sizeof(int) * Center);//initialization
    printf("ok\n");
    for (i = 0; i < Center; i++)
    {
        mpz_init_set_si(center_dis[i], 0);
        add[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
        center_new[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
        center_old[i] = (mpz_t*)malloc(sizeof(mpz_t) * Dim);
        for (j = 0; j < Dim; j++)
        {
            mpz_init(add[i][j]);
            mpz_init_set_si(center_new[i][j],0);
            mpz_init_set_si(center_old[i][j],0);
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
    //The client only needs a socket file descriptor to communicate with the server
    int clientSocket;
    //Describe the socket of the server
    struct sockaddr_in serverAddr;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    //Specify the IP address of the server, local test: 127.0.0.1
    //inet_ Addr() function to convert dotted decimal IP into network byte order IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect");
        return 1;
    }
    printf("Connect to host...\n");
    /*-----------Related variables and function end of communication----------------*/
    /*---------------------------
    --------The cycle begins-----------
    ---------------------------*/
    int loop=0,flag=0,byte,ret,midbyte;
    char transfer[1024],transfer1[1024];
    mpz_t x,y,max;
    mpz_init(x);
    mpz_init(y);
    mpz_init_set_si(max,10000);
    while(flag==0&&loop<100)
    {
        loop++;
        printf("\n\nThis is %d cycle!\n",loop);
        //接收center_new
        printf("Start receiving data center_ new\n");
        byte=0;
        k=0;
        for(i=0; i<Center; i++)
        {
            for(j=0; j<Dim; j++)
            {
                k++;
                memset(transfer,0,sizeof(transfer));
                midbyte=recv(clientSocket,transfer,sizeof(transfer),0);
                byte+=midbyte;
                if(midbyte!=1024)
                {
                    midbyte=recv(clientSocket,transfer1,sizeof(transfer1),0);
                    byte+=midbyte;
                }
                mpz_set_str(y,transfer,62);
                mpz_set(center_new[i][j],y);
             
            }
        }
        printf("Data received successfully %d\n",byte);
        //S0 Center_ Send new to S1, which means that S1 can obtain the centroid under plaintext, center_ new[][]
        //S1 starts to recalculate the distance from each data point to each centroid
        printf("  S1 start decryption!");
        decrypt_center(center_new,&priv1);
        printf_center(center_new);
        flag=cmp_old_new(center_new,center_old);
        //传输flag

        ret=send(clientSocket,&flag,sizeof(flag),0);
        if(flag==0)
        {
            copy_new_to_old(center_new,center_old);
            //center_ dis needs to be initialized,In the process of circulation, the modulus of each central point is calculated
            compute_mu(center_dis,center_new);
            //Encrypt the module of the center point with pk0
            encrypt_mu(center_dis,&pub0);
            initialization_dis(distence,center_dis,num_of_point);
            //S1 start to calculate the distance from each point to the center point!
            for (i = 0; i < num_of_point; i++)
            {
                for (j = 0; j < Center ; j++)
                {
                    for (k = 0; k < Dim; k++)
                    {
                        mpz_mul_si(mid2, center_new[j][k], 2);
                        mpz_sub(mid1, max, mid2);
                        paillier_homomorphic_multc(mid, data_pk0[i][k], mid1, &pub0);//Homomorphic multiplication:data_pk0*(n-2*center[j][k])
                        paillier_homomorphic_add(distence[i][j], distence[i][j], mid, &pub0);
                    }
                }
            }

            //The server S1 sends the relative distance encrypted with the public key pk0 to S0
            printf("开始发送distance\n");
            ret=0;
            for(i=0; i<num_of_point; i++)
            {
                for(j=0; j<Center; j++)
                {
                    memset(transfer,0,sizeof(transfer));
                    mpz_get_str(transfer,62,distence[i][j]);
                    ret+=send(clientSocket,transfer,sizeof(transfer),0);
                    if(ret<0)
                    {
                        printf("%d  ",ret);
                        break;
                    }

                }
            }
            printf("Successful delivery:distance  %d\n",ret);
        }
    }
    close(clientSocket);
    printf("Successfully closed communication\n");
    for (i = 0; i < num_of_point; i++)
    {
        free(*(data_pk0 + i));
    }//Free memory
    paillier_public_clear(&pub0);
    paillier_public_clear(&pub1);
    return 0;

}
//S1 decrypts and obtains the centroid of the plaintext
void decrypt_center(mpz_t** center_new,paillier_private_key* priv1)
{
    int i,j;
    for (i = 0; i < Center; i++)
    {
        for (j = 0; j < Dim; j++)
        {
            paillier_decrypt(center_new[i][j], center_new[i][j], priv1);
            mpz_add_ui(center_new[i][j],center_new[i][j],1);
            mpz_cdiv_q_ui(center_new[i][j],center_new[i][j],1000000);
            mpz_sub_ui(center_new[i][j],center_new[i][j],1);
        }
    }
}
//Compare the new center point with the old center point
int cmp_old_new(mpz_t** center_new,mpz_t** center_old)
{
    int i,j,flag=0;
    for(i=0; i<Center; i++)
    {
        for(j=0; j<Dim; j++)
        {
            if(mpz_cmp(center_new[i][j],center_old[i][j])!=0)
            {
                flag=1;
                break;
            }
        }
        if(flag)
            break;
    }
    if(flag==0)
        return 1;
    else
        return 0;
}
//Assign the new center point to the old center point
void copy_new_to_old(mpz_t** center_new,mpz_t** center_old)
{
    int i,j;
    for(i=0; i<Center; i++)
    {
        for(j=0; j<Dim; j++)
            mpz_set(center_old[i][j],center_new[i][j]);
    }
}
//compute module of center point
void compute_mu(mpz_t* center_dis,mpz_t** center_new)
{
    int i,j;
    mpz_t mid;
    mpz_init(mid);
    for(i=0; i<Center; i++)
        mpz_set_si(center_dis[i],0);
    for (i = 0; i < Center; i++)
    {
        for (j = 0; j < Dim; j++)
        {
            mpz_mul(mid, center_new[i][j], center_new[i][j]);
            mpz_add(center_dis[i], center_dis[i], mid);
        }
    }
}
//Encryption using public key pk0
void encrypt_mu(mpz_t* center_dis,paillier_public_key* pub0)
{
    int i;
    mpz_t mid;
    mpz_init(mid);
    for (i = 0; i < Center; i++)
    {
        paillier_encrypt(mid, center_dis[i], pub0);
        mpz_set(center_dis[i],mid);
    }
}
//initialization
void initialization_dis(mpz_t** distence,mpz_t* center_dis,int num_of_point)
{
    int i,j;
    for (i = 0; i < num_of_point; i++)
    {
        for (j = 0; j < Center; j++)
        {
            mpz_set(distence[i][j], center_dis[j]);//将距离初始化为pk0加密后的某质心的模
        }
    }
}
//Output function
void printf_center(mpz_t** center)
{
    int i,j;
    printf("解密后的中心点数据信息:\n");
    for(i=0; i<Center; i++)
    {
        printf("%d\n",i+1);
        for(j=0; j<Dim; j++)
        {
            gmp_printf("%Zd  ",center[i][j]);
            if((j+1)%24==0)
                printf("\n");
        }
        printf("\n");
    }
}
