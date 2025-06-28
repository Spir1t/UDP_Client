#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "CPU_Stats.h"
#include "CPU_StatsHandler.h"

/*
void print_cpu_stat(const CPU_Stat *stat) {
    printf("CPU Name     : %s\n", stat->Name);
    printf("  User       : %u\n", stat->User);
    printf("  Nice       : %u\n", stat->Nice);
    printf("  System     : %u\n", stat->System);
    printf("  Idle       : %u\n", stat->Idle);
    printf("  Iowait     : %u\n", stat->Iowait);
    printf("  Irq        : %u\n", stat->Irq);
    printf("  Softirq    : %u\n", stat->Softirq);
    printf("  Steal      : %u\n", stat->Steal);
    printf("  Guest      : %u\n", stat->Guest);
    printf("  Guest_nice : %u\n", stat->Guest_nice);
}

void print_cpu_stats(const CPU_Stats *stats) {
    printf("====== Common CPU Info ======\n");
    if (&stats->CPU) {
        print_cpu_stat(&(stats->CPU));
    }

    printf("\n====== CPU Cores (%u out of %u) ======\n", stats->Cores_Size, stats->Cores_Capacity);

    for (int i = 0; i < stats->Cores_Size; i++) {
        printf("\n--- Core %d ---\n", i);
        print_cpu_stat(&stats->CPU_Cores[i]);
    }
}

void PrintLoads(float load, float * loads, int size)
{
    printf("CPU Load: %f percent\n", load);
    for(int i = 0; i < size; i++)
    {
        printf("CPU%i Load: %f percent\n", i, loads[i]);
    }
    free(loads);
}
*/
int main()
{
    pthread_t sender_thread = start_sender_thread();
    if(sender_thread == 0)
    {
        perror("Failed to create thread");
        return 1;
    }
    pthread_join(sender_thread, NULL);
    return 0;
}