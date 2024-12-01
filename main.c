#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "string.h"

#define FIFO_NAME "./myfifo"
#define BUFF_SIZE 100

pid_t pid;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // khai bao khoa mutex lock dam bao su dung shared resource
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // khai bao cond

typedef struct {
    char name[30];
    char msg[30];
    char msg_connection[40];
    float temperature;
} shared_data;

static void *connection_handle(void *args)
{   
    shared_data *thr = (shared_data *)args;
    char buff[BUFF_SIZE] = "Connecting....";
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock);
    int fd = open(FIFO_NAME,O_WRONLY); // mo file fifo >> gan gia tri mo file = fd o parentprocess
        if(fd < 0){
            printf("Failed to open FIFO for writting.");
            exit(1);
        }
    printf("connection manager.\n");
    strncpy(thr->msg_connection,"Connecting.....",sizeof(thr->msg_connection));
    printf("%s\n",thr->msg_connection);
    write(fd, buff, strlen(buff) +1);
    sleep(3);
    strncpy(thr->msg_connection,"Connected",sizeof(thr->msg_connection));
    printf("%s\n",thr->msg_connection);
    close(fd);
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
        int fd;
        char msg[100] = {0}; // fd child_process
        printf("Im child process. %d\n",getpid());   
        if(fd = open(FIFO_NAME, O_RDONLY) < 0){
            printf("Failed to open FIFO for reading.");
            exit(1);
        }
        read(fd, msg, BUFF_SIZE);
        printf("msg: %s\n", msg);
        close(fd);
    }
    else{
        printf("Iam parent process. %d\n",getppid());
        pthread_t connection, data, storage_mng; // khai bao cac thread o req.2 
        shared_data shd; 
        pthread_create(&connection,NULL,&connection_handle,&shd);
        pthread_create(&data,NULL,&data_handle,&shd);
        pthread_create(&storage_mng,NULL,&storage_mng_handle,&shd);
        while(1)
        {}           
    }
}
