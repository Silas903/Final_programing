#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"

#define FIFO_NAME "myfifo"

pid_t pid;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // khai bao khoa mutex lock dam bao su dung shared resource
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // khai bao cond

typedef struct {
    char name[30];
    char msg[30];
    float temperature;
} shared_data;

static void *connection_handle(void *args)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock);
    printf("connection manager.\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);

}

static void *data_handle(void *args)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock);
    printf("data manager.\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

static void *storage_mng_handle(void *args)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock);
    printf("storage manager.\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(int argc,char const *argv[])
{
    if(access(FIFO_NAME,F_OK) == -1){
        if( mkfifo(FIFO_NAME,0666) != 0){
            printf(" mkfifo failed ");
            exit(1);
        }
        printf(" FIFO created by parent process.\n");
    } else {
        printf(" FIFO already exists.");
    }

    pid = fork(); // tao chuong trinh con 
    if(pid < 0)
    {
        perror("fork() fail !!");
        exit(1);
    }
    else if(pid == 0)
    {   
        printf("Im child process. %d\n",getpid());
        int fd = open(FIFO_NAME, O_RDONLY);
        if(fd < 0){
            printf("Failed to open FIFO for reading.");
            exit(1);
        } 
    }
    else{
        printf("Iam parent process. %d\n",getppid());
        pthread_t connection, data, storage_mng; // khai bao cac thread o req.2 
        shared_data shd;
        int fd = open(FIFO_NAME,O_WRONLY); // mo file fifo >> gan gia tri mo file = fd 
        if(fd < 0){
            printf("Failed to open FIFO for reading.");
            exit(1);
        } 
        pthread_create(&connection,NULL,&connection_handle,&shd);
        pthread_create(&data,NULL,&data_handle,&shd);
        pthread_create(&storage_mng,NULL,&storage_mng_handle,&shd);           
    }
}
