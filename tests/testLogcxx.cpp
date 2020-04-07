#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/xml/domconfigurator.h>
#include <pthread.h>

using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::xml;

static const string CONF_LOG_FILE_XML = "test.xml";
static const string CONG_LOG_FILE_PROPERTIES = "test.properties";
static const int NTHREADS = 10;
LoggerPtr logger(Logger::getRootLogger());

// log4cxx同步日志 单线程 100万 9.02045s 
// log4cxx同步日志 10线程各 50万 110.4303 
void* printfLog(void* arg)
{
    
    // # AsyncAppender
    for (int i = 0; i < 500/NTHREADS; i++)
    {
        for (int j = 0 ; j < 10000; j++)
        {
            LOG4CXX_INFO(logger, "fsfdsfasfafafasfffffsfasfs efsfasf wr fa efafarfrf");
        }
    }

    return nullptr;
}

// g++ -o log testLogcxx.cpp -L/usr/local/log4cxx/lib -llog4cxx -lexpat -I/usr/local/log4cxx/include -lpthread
int main(void)
{

    PropertyConfigurator::configure(CONG_LOG_FILE_PROPERTIES);
    // DOMConfigurator::configure(CONF_LOG_FILE_XML);
    time_t start, end;
    start = clock();

    pthread_t threads[NTHREADS];
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threads[i], nullptr, printfLog, nullptr);
    }


    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], nullptr);
    }
    // printfLog();

    end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
    cout<<"success"<<endl;

	return 0;
}

//log4j.appender.file.BufferedIO=true
// log4j.appender.file.BufferSize=10240

//log4j.appender.asyn_file=org.apache.log4j.AsyncAppender
// log4j.appender.asyn_file.appender-ref = log4j.appender.file