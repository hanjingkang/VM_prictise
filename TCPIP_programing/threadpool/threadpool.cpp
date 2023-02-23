/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2023-01-05 21:42:52
 * @LastEditTime: 2023-01-07 20:29:19
 */
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define DEFAULT_TIME 10        //10s检测一次
#define MIN_WAIT_TASK_NUM 10   //若queue_size大于之，添加新线程到线程池
#define DEFAULT_THREAD_VARY 10 //每次创建销毁的线程个数
#define true 1
#define false 0
using namespace std;

typedef struct
{
    void *(*function)(void *); //函数指针 ，回调函数
    void *arg;                 //函数的参数
} threadpool_task_t;           //各子线程任务结构体

//线程池结构体：
struct threadpool_t
{
    pthread_mutex_t lock;           //锁住本结构体
    pthread_mutex_t thread_counter; //记录忙状态的线程个数

    pthread_cond_t queue_not_full;  //任务队列满时，添加任务的线程阻塞，等待此条件变量
    pthread_cond_t queue_not_empty; //任务队列不为空时，通知等待任务的线程

    pthread_t *threads;            //存放线程池中每个线程的tid，数组
    pthread_t admin_tid;           //存管理线程的tid
    threadpool_task_t *task_queue; //任务队列（数组首地址）

    int min_thr_num;       //线程池最小线程数
    int max_thr_num;       //线程池最大线程数
    int live_thr_num;      //当前存活线程数
    int busy_thr_num;      //当前 忙线程个数
    int wait_exit_thr_num; //要销毁的线程个数

    int queue_front;    //任务队头下标
    int queue_rear;     //任务 队尾下标
    int queue_size;     //任务队列实际任务数
    int queue_max_size; //任务队列最大容量

    int shutdown; //标志位，线程使用状态，true/false
};

void *threadpool_thread(void *threadpool);

void *admin_thread(void *threadpool);

//创建线程池
threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    threadpool_t *pool; //线程池结构体
    do
    {
        if ((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
        {
            printf("malloc threadpool failed\n");
            break;
        }

        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num = 0;
        pool->live_thr_num = min_thr_num; //活着的线程数初值=最小线程数
        pool->wait_exit_thr_num = 0;
        pool->queue_size = 0;                  //现有任务
        pool->queue_max_size = queue_max_size; //队列最大容量
        pool->queue_front = 0;
        pool->queue_rear = 0;
        pool->shutdown = false; //不关闭线程池

        //根据最大线程数,给工作线程数组开辟空间，并清零
        pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * max_thr_num);
        if (pool->threads = NULL)
        {
            printf("malloc thread failed\n");
            break;
            /* code */
        }
        memset(pool->threads, 0, sizeof(pthread_t) * max_thr_num);

        //给任务队列开辟空间
        pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_max_size);
        if (pool->task_queue = NULL)
        {
            printf("malloc task_queue failed\n");
            break;
        }
        //初始化互斥锁、条件变量
        if (pthread_mutex_init(&(pool->lock), NULL) != 0 || pthread_mutex_init(&(pool->thread_counter), NULL) != 0 || pthread_cond_init(&(pool->queue_not_empty), NULL) != 0 || pthread_cond_init(&(pool->queue_not_full), NULL) != 0)
        {
            printf("init mutex/cond failed\n");
            break;
        }

        //启动min_thr_num个work thread
        for (i = 0; i < min_thr_num; i++)
        {
            pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool); //pool指向当前线程池
            printf("start thread 0x%x...\n", (unsigned int)pool->threads[i]);
            /* code */
        }
        pthread_create(&(pool->admin_tid), NULL, admin_thread, , (void *)pool); //创建 管理者线程

        return pool;

    } while (0);
    threadpool_free(pool);
    return NULL;
}

//线程池中各个工作线程
void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;
    while (true)
    {
        //刚创建出线程，等待任务队列里有任务，否则一直阻塞等待任务队列里有任务在唤醒接收任务
        pthread_mutex_lock(&(pool->lock));

        //queue_size=0说明没有任务，调用wait阻塞在条件变量上，若有任务则跳过该while
        while ((pool->queue_size == 0) && (!pool->shutdown))
        {
            printf("thread 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
            /* code */
            //清除指定数目的空闲线程，数目大于0则执行
            if (pool->wait_exit_thr_num > 0)
            {
                pool->wait_exit_thr_num--;
                /* code */

                //如果线程池里线程个数大于最小值时则结束当前线程
                if (pool->live_thr_num > pool->min_thr_num)
                {
                    printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
                    pool->live_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));
                    pthread_exit(NULL);
                    /* code */
                }
            }
        }

        //如果指定了true，要关闭线程池里每个线程，自行退出处理--销毁线程池
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&(pool->lock));
            printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
            pthread_detach(pthread_self());
            pthread_exit(NULL);
            /* code */
        }

        //从任务队列里获取任务，是一个出队操作
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;

        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
        pool->queue_size--;

        //通知可以有新的任务添加进来
        pthread_cond_broadcast(&(pool->queue_not_full));

        //任务取出后将线程池释放
        pthread_mutex_unlock(&(pool->lock));

        //执行任务
        printf("thread 0x%x start working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&(pool->busy_thr_num));
        pool->busy_thr_num++;
        pthread_mutex_unlock(&(pool->busy_thr_num));

        //执行回调函数
        (*(task.function))(task.arg);

        //任务结束处理
        printf("thread 0x%x end working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&(pool->busy_thr_num));
        pool->busy_thr_num--;
        pthread_mutex_unlock(&(pool->busy_thr_num));
    }
    pthread_exit(NULL);
}

//管理者线程
void *admin_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t *)threadpool;
    while (!pool->shutdown)
    {
        sleep(DEFAULT_TIME);
        /* code */
        pthread_mutex_lock(&(pool->lock));
        int queue_size = pool->queue_size;
        int live_num = pool->live_thr_num;
        pthread_mutex_unlock(&(pool->lock));

        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thr_num = pool->busy_thr_num;
        pthread_mutex_unlock(&(pool->thread_counter));

        //创建新线程的算法：任务数大于最小线程池现场个数，且存活的线程数少于最大线程数时
        if (queue_size >= MIN_WAIT_TASK_NUM && live_num < pool->max_thr_num)
        {
            pthread_mutex_lock(&(pool->lock));
            int add = 0;
            /* code */

            //一次添加DEFAULT_THREAD个线程
            for (i = 0; add < DEFAULT_THREAD_VARY && i < pool->max_thr_num && pool->live_thr_num < pool->max_thr_num; i++)
            {
                if (pool->threads[i] == 0 || !is_thread_alive(pool->threads[i]))
                {
                    pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool);
                    add++;
                    pool->live_thr_num++;
                    /* code */
                }

                /* code */
            }
            pthread_mutex_unlock(&(pool->lock));
        }

        //销毁多余线程的：两倍的忙线程数小于存活线程数并且
        if ((busy_thr_num * 2) < live_num && live_num > pool->min_thr_num)
        {
            //一次销毁DEFAULT_THREAD个线程，随机10个即可
            pthread_mutex_lock(&(pool->lock));
            pool->wait_exit_thr_num = DEFAULT_THREAD_VARY;
            pthread_mutex_unlock(&(pool->lock));
            /* code */

            for (i = 0; i < DEFAULT_THREAD_VARY; i++)
            {
                //通知除在空闲状态的线程，他们会自行终止
                /* code */
                pthread_cond_signal(&(pool->queue_not_empty));
            }
        }
    }
    return NULL;
}

int threadpool_destroy(threadpool_t *pool)
{
    int i;
    if (pool == NULL)
    {
        return -1;
        /* code */
    }
    pool->shutdown = true;

    //销毁 管理线程
    pthread_join(pool->admin_tid, NULL);

    //通知所有空闲线程
    for (i = 0; i < pool->live_thr_num; i++)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
        /* code */
    }

    for (i = 0; i < pool->live_thr_num; i++)
    {
        pthread_join(pool->threads[i], NULL);
        /* code */
    }

    threadpool_free(pool);

    return 0;
}

int threadpool_free(threadpool_t *pool)
{
    if (pool = NULL)
    {
        return -1;
        /* code */
    }
    if (pool->task_queue)
    {
        free(pool->task_queue);
        /* code */
    }

    if (pool->threads)
    {
        free(pool->threads);
        /* code */
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_mutex_lock(&(pool->thread_counter));
        pthread_mutex_destroy(&(pool->thread_counter));
        pthread_cond_destroy(&(pool->queue_not_empty));
        pthread_cond_destroy(&(pool->queue_not_full));
    }

    free(pool);
    pool = NULL;

    return 0;
}

//线程池中的线程，模拟处理业务
void *process(void *arg)
{
    printf("thread 0x%x working on task\n", (unsigned int)pthread_self(), (int)arg);
    sleep(1);
    prinf("task %d is end\n", (int)arg);

    return NULL;
}
//向线程池中加入一个线程
int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void *arg)
{
    pthread_mutex_lock(&(pool->lock));

    while ((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
    {
        pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
        /* code */
    }

    if (pool->shutdown)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
        pthread_mutex_unlock(&(pool->lock));
        return 0;
        /* code */
    }
    //清空工作线程调用的回调函数都参数arg
    if (pool->task_queue[pool->queue_rear].arg != NULL)
    {
        pool->task_queue[pool->queue_rear].arg = NULL;
        /* code */
    }

    //添加任务到队列
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size; //队尾指针移动，模拟环形
    pool->queue_size++;

    //添加完任务后，队列不为空，唤醒线程池中等待处理任务的线程
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));

    return 0;
}

int main(int argc, char const *argv[])
{
    threadpool_t *thp = threadpool_create(3, 100, 100); //创建线程池，最少3线程，最多100，队列容量100
    std::cout << "pool inited" << std::endl;

    int num[20], i;
    for (i = 0; i < 20; i++)
    {
        num[i] = i;
        printf("add task:%d", i);

        threadpool_add(thp, process, (void *)&num[i]);
        /* code */
    }
    sleep(10);
    threadpool_destroy(thp);

    return 0;
}
