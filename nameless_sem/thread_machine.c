/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-09 00:56:02
 * @LastEditors: northward
 * @LastEditTime: 2019-02-09 02:40:52
 * @Description: 
 *  使用无名信号量实现三线程循环同步状态机
 *  模拟 取数据->处理数据->输出数据
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 256

//用于传入线程参数的
typedef struct arg_t
{
    char* buf;
    sem_t sem;
};

//取数据线程
void *read_thread(void *arg);
//处理数据线程
void *handle_thread(void *arg);
//回显结果线程
void *write_thread(void *arg);

/**
 * @description: 主函数,创建并启动三个线程
 * @param {参数个数, 参数} 
 * @return: 错误码
 */
int main(int argc, char const *argv[])
{
    char buf[BUF_SIZE]="\0"; //公共数据
    sem_t sem;          //线程共享信号量
    pthread_t tid[3]    //三个线程id
    int ret;
    int *pthread_ret;
    arg_t arg
    {
        .sem=sem;
        .buf=buf;
    };
    
    //初始化线程间共享初值为1的信号量
    ret = sem_init(sem, 0, 1);
    if (ret < 0)
        handle_error("sem_init err");

    //id,属性,线程主函数,线程主函数参数
    pthread_create(&tid[1], NULL, read_thread, &arg);
    pthread_create(&tid[1], NULL, handle_thread, &arg);
    pthread_create(&tid[1], NULL, write_thread, &arg);
    if (tid[1] < 0 || tid[2] < 0 | tid[3] < 0)
        handle_error("pthread_create err");

    //启动线程并等待线程结束
    for (int i = 0; i < 3; ++i)
    {
        pthread_join(tid[i], &pthread_ret); //线程id和保存返回值的位置
        if (*pthread_ret < 0)
            printf("thread %d handle err\n", i);
    }
    printf("End of all threads\n");

    return 0;
}

/**
 * @description: 取数据线程
 * @param {type} 
 * @return: 成功返回读取大小, 失败返回-errno
 */
void *read_thread(void *arg)
{
    arg_t* in=(arg_t*)arg;
    sem_t sem=in->sem;
    char* buf=in->buf;
    int* pret=(int)malloc(sizeof(int));
    *pret=0;

    if(sem_wait(&sem)<0);//P
        *pret=-errno;
    printf("Before read thread sem: %d, buf: %s", sem, buf);
    strcat(buf,"Data reading...\n"); //模拟从设备文件进行数据读取
    printf("After read thread sem: %d, buf %s\n", sem, buf);
    if(sem_post(&sem)<0);//V
        *pret=-errno;
    
    return pret;
}

/**
 * @description: 处理数据线程
 * @param {type} 
 * @return: 
 */
void *handle_thread(void *arg)
{
    
}

/**
 * @description: 回显结果线程
 * @param {type} 
 * @return: 
 */
void *write_thread(void *arg)
{


}

