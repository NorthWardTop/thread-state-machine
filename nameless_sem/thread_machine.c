/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-09 00:56:02
 * @LastEditors: northward
 * @LastEditTime: 2019-02-09 16:15:50
 * @Description: 
 *  使用无名信号量实现三线程循环同步状态机
 *  模拟 取数据->处理数据->输出数据
 * 
 *  sem r=1,h=0,w=0
 *  read:       handle:     write:
 *  P(r)        P(h)        P(w)      
 *  read()      handle()    write()
 *  V(h)        V(w)        V(r)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include "myGeneral.h"

#define BUF_SIZE 256

//线程间共享的信号量
sem_t read_sem;
sem_t handle_sem;
sem_t write_sem;

//取数据线程
void *read_thread(void *arg);
//处理数据线程
void *handle_thread(void *arg);
//回显结果线程
void *write_thread(void *arg);
//显示信号量和数据缓冲区信息
void show_buf(char *buf);

/**
 * @description: 主函数,创建并启动三个线程
 * @param {参数个数, 参数} 
 * @return: 错误码
 */
int main(int argc, char const *argv[])
{
    char buf[BUF_SIZE]; //公共数据
    pthread_t tid[3];   //三个线程id
    int ret;
    int *pthread_ret; //线程返回指针

    //初始化缓冲区
    memset(buf, '\0', sizeof(buf));
    //初始化三个信号量, 线程间共享, 读初值1, 处理和写初值为0
    sem_init(&read_sem, 0, 1);
    sem_init(&handle_sem, 0, 0);
    ret = sem_init(&write_sem, 0, 0);
    if (ret < 0)
        handle_error("sem_init failed");

    //id,属性,线程主函数,线程主函数参数
    pthread_create(&tid[1], NULL, read_thread, buf);
    pthread_create(&tid[1], NULL, handle_thread, buf);
    pthread_create(&tid[1], NULL, write_thread, buf);
    if (tid[1] < 0 || tid[2] < 0 || tid[3] < 0)
        handle_error("pthread_create failed");

    //启动线程并等待线程结束
    for (int i = 0; i < 3; ++i)
    {
        pthread_join(tid[i], (void*)&pthread_ret); //线程id和保存返回值的位置
        if (*pthread_ret < 0)
            printf("thread %d handle failed\n", i);
    }
    printf("End of all threads\n");
    if (pthread_ret != NULL)
        free(pthread_ret);
    pthread_ret = NULL;

    return 0;
}

/**
 * @description: 取数据线程
 * @param {type} 
 * @return: 成功返回0, 失败返回-errno
 */
void *read_thread(void *arg)
{
    char *buf = (char *)arg;
    int *pret = (int *)malloc(sizeof(int));
    //内存用于返回
    if (pret == NULL)
        handle_error("read thread alloc memory failed");
    *pret = 0;

    if (sem_wait(&read_sem) < 0) //P(r)
    {
        show_msg("read thread P(read_sem) failed");
        *pret = -errno;
    }
    printf("Before read thread sem and buf:\n");
    show_buf(buf);
    strcat(buf, "Data reading...\n"); //模拟从设备文件进行数据读取
    printf("After read thread sem and buf:\n");
    show_buf(buf);
    if (sem_post(&handle_sem) < 0) //V(h)
    {
        show_msg("read thread V(handle_sem) failed");
        *pret = -errno;
    }

    return pret;
}

/**
 * @description: 处理数据线程
 * @param {线程参数} 
 * @return: 成功返回0, 失败返回-errno
 */
void *handle_thread(void *arg)
{
    char *buf = (char *)arg;
    int *pret = (int *)malloc(sizeof(int));
    //内存用于返回
    if (pret == NULL)
        handle_error("handle thread alloc memory failed");
    *pret = 0;

    if (sem_wait(&handle_sem) < 0) //P(h)
    {
        show_msg("handle thread P(handle_sem) failed");
        *pret = -errno;
    }
    printf("Before handle thread sem and buf:\n");
    show_buf(buf);
    strcat(buf, "Data processing...\n"); //模拟数据处理
    printf("After handle thread sem and buf:\n");
    show_buf(buf);
    if (sem_post(&write_sem) < 0) //V(w)
    {
        show_msg("handle thread V(write_sem) failed");
        *pret = -errno;
    }

    return pret;
}

/**
 * @description: 回显结果线程
 * @param {线程参数} 
 * @return: 成功返回0, 失败返回-errno
 */
void *write_thread(void *arg)
{
    char *buf = (char *)arg;
    int *pret = (int *)malloc(sizeof(int));
    //内存用于返回
    if (pret == NULL)
        handle_error("write thread alloc memory failed");
    *pret = 0;

    if (sem_wait(&write_sem) < 0) //P(w)
    {
        show_msg("write thread P(write_sem) failed");
        *pret = -errno;
    }
    printf("Before write thread sem and buf:\n");
    show_buf(buf);
    strcat(buf, "Data writing...\n"); //模拟数据写到输出设备
    printf("After write thread sem and buf:\n");
    show_buf(buf);
    if (sem_post(&read_sem) < 0) //V(r)
    {
        show_msg("handle thread V(read_sem) failed");
        *pret = -errno;
    }

    return pret;
}

/**
 * @description: 显示信号量和数据缓冲区信息
 * @param 缓冲区 
 * @return: 
 */
void show_buf(char *buf)
{
    int r,h,w;
    int ret;
    ret=sem_getvalue(&read_sem, &r);
    if(ret<0)
        handle_error("sem_getvalue failed");
    sem_getvalue(&handle_sem, &h);
    sem_getvalue(&handle_sem, &w);
    printf("read_sem: %d\thandle_sem: %d\twrite_sem: %d\n", r, h, w);
    printf("Buffer: %s\n", buf);
}
