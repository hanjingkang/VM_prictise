/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-04 14:46:56
 * @LastEditTime: 2022-11-04 14:58:51
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    int status;
    if (pid == 0)
    {
        return 3;
        
    }
    else
    {
        std::cout << "parent process here ,read child PID:" << pid << std::endl;
        pid = fork();
        if (pid == 0)
        {
            exit(7);
            
        }
        else
        {
            std::cout << "sub parent process here ,read child PID:" << pid << std::endl;
            wait(&status);
            if (WIFEXITED(status))
            {
                std::cout << "child send:" << WEXITSTATUS(status) << std::endl;
                
            }
            wait(&status);
            if (WIFEXITED(status))
            {
                std::cout << "child send:" << WEXITSTATUS(status) << std::endl;
                
            }
        }
    }
    return 0;
}