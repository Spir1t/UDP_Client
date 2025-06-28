#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "CPU_Stats.h"
#include "CPU_StatsHandler.h"

CPU_Stats * GetStats()
{
    CPU_Stats * Stats = CPU_Stats_Init();
    FILE * file = fopen("/proc/stat", "r");
    if(!file)
    {
        perror("Cannot read /proc/stat");
        return NULL;
    }

    size_t size = 0;
    size_t capacity = BUFFER_SIZE;
    char *Buf = (char*)malloc(capacity);
    if(!Buf)
    {
        perror("Allocation error");
        fclose(file);
        return NULL;
    }

    int c;
    while((c = fgetc(file)) != EOF)
    {
        Buf[size] = c;
        size++;
        if(size >= capacity)
        {
            capacity *= ALLOC_MUL_COEFF;
            Buf = realloc(Buf, capacity);
            if(!Buf)
            {
                perror("Allocation error");
                fclose(file);
                return NULL;
            }
        }
    }
    Buf[size] = '\0';

    unsigned int pos = 0;
    Read_CPU_Stat(&(Stats->CPU), Buf , &pos);
    Read_CPU_Cores(Stats, Buf, &pos);

    free(Buf);
    fclose(file);
    return Stats;
}

float GetCurrentCpuUsage()
{
    CPU_Stats * Stats1 = GetStats();
    CPU_Stat Stat1 = Stats1->CPU;
    sleep(1);
    CPU_Stats * Stats2 = GetStats();
    CPU_Stat Stat2 = Stats2->CPU;
    float ActiveTime1 = Stat1.User + Stat1.Nice + Stat1.System + Stat1.Irq + Stat1.Softirq + Stat1.Steal;
    float IdleTime1 = Stat1.Idle + Stat1.Iowait;
    float ActiveTime2 = Stat2.User + Stat2.Nice + Stat2.System + Stat2.Irq + Stat2.Softirq + Stat2.Steal;
    float IdleTime2 = Stat2.Idle + Stat2.Iowait;
    float ActiveDiff = ActiveTime2 - ActiveTime1;
    float IdleDiff = IdleTime2 - IdleTime1;
    CPU_Stats_Deinit(Stats1);
    CPU_Stats_Deinit(Stats2);

    return ActiveDiff / (IdleDiff + ActiveDiff) * 100;
}

float GetCPU_Usage(CPU_Stat Before, CPU_Stat After)
{
    float ActiveTimeBefore = Before.User + Before.Nice + Before.System + Before.Irq + Before.Softirq + Before.Steal;
    float IdleTimeBefore = Before.Idle + Before.Iowait;
    float ActiveTimeAfter = After.User + After.Nice + After.System + After.Irq + After.Softirq + After.Steal;
    float IdleTimeAfter = After.Idle + After.Iowait;
    float ActiveDiff = ActiveTimeAfter - ActiveTimeBefore;
    float IdleDiff = IdleTimeAfter - IdleTimeBefore;
    return ActiveDiff / (IdleDiff + ActiveDiff) * 100;
}

float * GetCurrentCoresUsage()
{
    CPU_Stats * Stats1 = GetStats();
    sleep(1);
    CPU_Stats * Stats2 = GetStats();
    float * Loads = (float*)calloc(Stats1->Cores_Size, sizeof(float));
    for(int i = 0 ; i < Stats1->Cores_Size; i++)
    {
        Loads[i] = GetCPU_Usage(Stats1->CPU_Cores[i], Stats2->CPU_Cores[i]);
    }
    CPU_Stats_Deinit(Stats1);
    CPU_Stats_Deinit(Stats2);
    return Loads;
}

float * GetCoresUsage(CPU_Stats * Before, CPU_Stats * After)
{
    float * Loads = (float*)calloc(Before->Cores_Size, sizeof(float));
    for(int i = 0 ; i < Before->Cores_Size; i++)
    {
        Loads[i] = GetCPU_Usage(Before->CPU_Cores[i], After->CPU_Cores[i]);
    }
    return Loads;
}

size_t GetCoresNumber()
{
    CPU_Stats * Stats1 = GetStats();
    if(!Stats1) {perror("Failed to read /proc/stat");}
    size_t Num = Stats1->Cores_Size;
    CPU_Stats_Deinit(Stats1);
    return Num;
}

void* send_loop(void* arg)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        pthread_exit(NULL);
    }

    struct sockaddr_in Dest;
    Dest.sin_family = AF_INET;
    Dest.sin_port = htons(DEST_PORT);
    Dest.sin_addr.s_addr = inet_addr(DEST_IP);

    CPU_Stats * Before = GetStats();
    CPU_Stats * After;

    while (1) {
        sleep(1);
        After = GetStats();
        if(!After) {perror("Failed to read proc/stat");}

        float CPU_Usage = GetCPU_Usage(Before->CPU, After->CPU);
        float * Cores_Usage = GetCoresUsage(Before, After);
        size_t Cores_Num = GetCoresNumber();

        float buffer[1 + Cores_Num];
        buffer[0] = CPU_Usage;
        memcpy(&buffer[1], Cores_Usage, sizeof(float) * Cores_Num);
        free(Cores_Usage);

        ssize_t sent = sendto(
            sock, buffer, sizeof(float) * (1 + Cores_Num), 0,
            (struct sockaddr*)&Dest, sizeof(Dest)
        );

        if (sent < 0) {
            perror("sendto");
        } else {
            printf("Sent %li byte\n", sent);
        }

        CPU_Stats_Deinit(Before);
        Before = After;
    }

    return NULL;
}

pthread_t start_sender_thread()
{
    pthread_t thread;

    if(pthread_create(&thread, NULL, send_loop, NULL) != 0)
    {
        perror("pthread_create failed");
        return (pthread_t){0};
    }
    return thread;
}