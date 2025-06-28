#pragma once
#include <arpa/inet.h>
#include <pthread.h>
#include "CPU_Stats.h"

#define DEST_PORT 1234
#define DEST_IP "127.0.0.1"


typedef struct UdpSenderContext
{
    int sockfd;
    struct sockaddr_in addr;
} UdpSenderContext;

//Read from /proc/stat
CPU_Stats * GetStats();

//CPU load handling
float GetCurrentCpuUsage();
float GetCPU_Usage(CPU_Stat Before, CPU_Stat After);
float * GetCurrentCoresUsage();
float * GetCoresUsage(CPU_Stats * Before, CPU_Stats * After);
size_t GetCoresNumber();

//Send loop
void *send_loop(void *arg);
pthread_t start_sender_thread();