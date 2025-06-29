#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "CPU_Stats.h"
#include "CPU_StatsHandler.h"

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