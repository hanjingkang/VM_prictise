/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-04 14:46:56
 * @LastEditTime: 2022-11-04 15:50:33
 */
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>

using namespace std;

void read_childProc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        std::cout << "remove proc:" << id << std::endl;
        
        std::cout << "child sent:" << WEXITSTATUS(status) << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childProc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();

    if (pid == 0)
    {
        std::cout << "child here" << std::endl;
        sleep(10);
        return 3;
        
    }
    else
    {
        std::cout << "parent process here ,read child PID:" << pid << std::endl;
        pid = fork();
        if (pid == 0)
        {
            std::cout << "child here" << std::endl;
            sleep(10);
            return 4;
        }
        else
        {
            int i;
            std::cout << "sub parent process here ,read child PID:" << pid << std::endl;
            for (size_t i = 0; i < 5; i++)
            {
                std::cout << "wait.." << std::endl;
                
                sleep(5);
            }
            

        }
    }
    return 0;
}