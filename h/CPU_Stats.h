#pragma once

#define DEFAULT_CORES_NUM 4
#define BUFFER_SIZE 1024
#define ALLOC_MUL_COEFF 2

typedef struct CPU_Stat
{
    char Name[64];
    unsigned int User;
    unsigned int Nice;
    unsigned int System;
    unsigned int Idle;
    unsigned int Iowait;
    unsigned int Irq;
    unsigned int Softirq;
    unsigned int Steal;
    unsigned int Guest;
    unsigned int Guest_nice;
} CPU_Stat;

typedef struct CPU_Stats
{
    CPU_Stat CPU;
    CPU_Stat * CPU_Cores;
    size_t Cores_Size;
    size_t Cores_Capacity;
} CPU_Stats;

//CPU_Stat handling
CPU_Stat * CPU_Stat_Init();
void CPU_Stat_Deinit(CPU_Stat * Stat);
void Read_CPU_Stat(CPU_Stat * Dest, char * Src, unsigned int * startPos);

//CPU_Stats handling
CPU_Stats * CPU_Stats_Init();
void CPU_Stats_Deinit(CPU_Stats * Stats);
void Read_CPU_Cores(CPU_Stats * Dest, char * Src, unsigned int * Pos);
void Reallocate_Cores(CPU_Stats * Stats);