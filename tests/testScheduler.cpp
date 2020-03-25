#include "../sylar.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;
    SYLAR_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;
     sylar::Scheduler::GetThis()->schedule([](){
        sleep(2);
        SYLAR_LOG_INFO(g_logger) << "sleep 2";
    });
    // sleep(1);
    // if(--s_count >= 0) {
    //     sylar::Scheduler::GetThis()->schedule(&test_fiber, sylar::GetThreadId());
    // }
}

int main(int argc, char** argv) {
    SYLAR_LOG_INFO(g_logger) << "main";
    sylar::Scheduler sc(1, true, "test");
    SYLAR_LOG_DEBUG(g_logger) << "start";
    sc.start();
    SYLAR_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    SYLAR_LOG_DEBUG(g_logger) << "stop";
    sc.stop();
    SYLAR_LOG_INFO(g_logger) << "over";
    return 0;
}