#include <pthread.h>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <time.h>
#include <unistd.h>

using namespace std;

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void* threadFunc(void* args)
{
    for (int i = 0; i < 1000; i++)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);
        long tmp = 0;
        for (int j = 0; j < 20000; j++)
        {
            
            tmp++;
        }
        usleep(20);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
    }

    pthread_exit((void *)0);
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);

    vector<pthread_t> threads;
    clock_t t1 = clock();
    for (int i = 0; i < 10; i++)
    {
        pthread_t p;
        pthread_create(&p, nullptr, threadFunc, (void*)0);
        threads.push_back(p);
    }

    for (int i = 0; i < threads.size(); i++)
    {
        pthread_join(threads[i], nullptr);
    }
    cout << (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms" << endl;

    cout << "end" << endl;
}