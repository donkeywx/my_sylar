#include <iostream>
#include <pthread.h>
#include <vector>

pthread_cond_t cond;
pthread_mutex_t mutex;
int gvar = 0;

void* run(void* args)
{
    long num = (long) args;
    
    int cycs = 3;
    for (int i = 0; i < cycs; i++)
    {
        pthread_mutex_lock(&mutex);

        while(num != gvar)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        
        std::cout << gvar << std::endl;
        gvar++;
        gvar = gvar % 10;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
    }

    return (void*)0;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    std::vector<pthread_t> threads;
    for (int i = 0; i < 10; i++)
    {
        pthread_t thread = 0;
        pthread_create(&thread, nullptr, run, (void*)i);
        threads.push_back(thread);
    }

    for (int i = 0; i < threads.size(); i++)
    {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "end" << std::endl;
    return 0;
}