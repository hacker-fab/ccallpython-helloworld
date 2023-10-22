#include <iostream>
#include <cstdlib>
#include <ctime>
#include "submodule.hpp"

//cross-platform implementation
#ifdef _WIN32
#include <Windows.h>
//TODO: #include windows pthread implementation
#define SLEEP(x) Sleep(x * 1000)
#else
#include <pthread.h>
#include <unistd.h>
#define SLEEP(x) sleep(x)
#endif

//global variables

struct DataFormat {
    int x;
    int y;
    int z;
} data;

Submodule submodule(10);
void (*dataCallback)(char *data);
bool shouldExit;

int pthreadReturn;

//external functions

extern "C" float testFromPy(int val) {
    return 2.5 * submodule.test(val);
}

extern "C" void setDataCallback(void (*callback)(char *data)) {
    dataCallback = callback;
}

extern "C" void exitCpp() {
    shouldExit = true;
}

//internal functions

void updateData() {
    data.x = (int) rand() % 100 - 100;
    data.y = (int) rand() % 100 - 50;
    data.z = (int) rand() % 100 - 0;

    if(dataCallback) {
        dataCallback(reinterpret_cast<char *>(&data));
    }
}

void *newRoutine(void *data) {
    std::cout << "[C++] New thread started successfully." << std::endl;

    while(!shouldExit) {
        SLEEP(3);
        updateData();
    }

    pthreadReturn = 0;
    shouldExit = false;
    std::cout << "[C++] Secondary thread exited successfully." << std::endl;
    pthread_exit(&pthreadReturn);
}

//entry point; also external

int main(int argc, char *argv[]) {
    bool startNewThread = false;
    pthread_t threadId;
    void *pthreadReturnPtr = &pthreadReturn;

    std::cout << "[C++] main function called." << std::endl;

    //bad parsing but it works for this example
    if(argc == 2 && argv[1][0] == '1') { 
        startNewThread = true;
        std::cout << "[C++] Starting new thread..." << std::endl;
    }
    else {
        std::cout << "[C++] Not starting new thread." << std::endl;
    }

    shouldExit = false;
    dataCallback = nullptr;
    srand(time(nullptr));
    data.x = 0;
    data.y = 0;
    data.z = 0;

    if(startNewThread) {
        pthread_create(&threadId, nullptr, newRoutine, nullptr);
        //pthread_join(threadId, reinterpret_cast<void **>(&pthreadReturnPtr));
    }

    std::cout << "[C++] Original thread exited successfully." << std::endl;
    return 0;
}
