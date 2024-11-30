#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"

pid_t pid;
pthread_t connection, data, storage_mng; // khai bao cac thread ow req.2  
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // khai bao khoa mutex lock dam bao su dung shared resource
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // khai bao cond

typedef struct {
    char name[30];
    char msg[30];
} shared_data;

static void *connection_handle(void *args)
{
    pthread_detach();
    pthread_mutex_lock(&lock);
    print("connection manager");
    pthread_mutex_unlock(&lock);
    pthread_exit();

}

static void *data_handle(void *args)
{
    pthread_detach();
    pthread_mutex_lock(&lock);
    print("data manager");
    pthread_mutex_unlock(&lock);
    pthread_exit();
}

static void *storage_mng_handle(void *args)
{
    pthread_detach();
    pthread_mutex_lock(&lock);
    print("storage manager");
    pthread_mutex_unlock(&lock);
    pthread_exit();
}

int main(int argc,char const *argv[])
{
    pid = fork(); // tao chuong trinh con 
    if(pid < 0)
    {
        perror("fork() fail !!");
        exit();
    }
    elif (pid == 0)
    {
        print("Im child process. %d",getpid()); 
    }
    else{
        print("Iam parent process. %d",getppid());
        
    }
}
