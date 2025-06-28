#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CPU_Stats.h"

static char * ReadToken(char * Src, unsigned int * pos)
{
    while(Src[*pos] && isspace(Src[*pos]))
    {
        (*pos)++;
    }

    char * Token = calloc(64, sizeof(char));
    if (!Token) return NULL;

    unsigned int TokenPos = 0;
    while(Src[*pos] && !isspace(Src[*pos]))
    {
        Token[TokenPos] = Src[*pos];
        TokenPos++;
        (*pos)++;
    }
    Token[TokenPos] = '\0';

    while(Src[*pos] && isspace(Src[*pos]))
    {
        (*pos)++;
    }
    return Token;
}

CPU_Stat * CPU_Stat_Init()
{
    CPU_Stat * Stat = calloc(1, sizeof(CPU_Stat));
    memset(Stat, 0, sizeof(Stat));
    return Stat;
}

void CPU_Stat_Deinit(CPU_Stat * Stat)
{
    free(Stat);
}

CPU_Stats * CPU_Stats_Init()
{
    CPU_Stats * Stats = (CPU_Stats *)calloc(1, sizeof(CPU_Stats));
    Stats->Cores_Size = 0;
    Stats->Cores_Capacity = DEFAULT_CORES_NUM;
    memset(&Stats->CPU, 0, sizeof(CPU_Stat));
    Stats->CPU_Cores = (CPU_Stat*)calloc(Stats->Cores_Capacity, sizeof(CPU_Stat));
    return Stats;
}

void CPU_Stats_Deinit(CPU_Stats * Stats)
{
    CPU_Stat_Deinit(Stats->CPU_Cores);
    free(Stats);
}

void Read_CPU_Stat(CPU_Stat * Dest, char * Src, unsigned int * startPos)
{
    char * Token = ReadToken(Src, startPos);
    strncpy(Dest->Name, Token, 16);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->User = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Nice = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->System = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Idle = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Iowait = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Irq = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Softirq = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Steal = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Guest = atoi(Token);
    free(Token);

    Token = ReadToken(Src, startPos);
    Dest->Guest_nice = atoi(Token);
    free(Token);
}

void Read_CPU_Cores(CPU_Stats * Dest, char * Src, unsigned int * Pos)
{
    CPU_Stat * Cores_ptr = Dest->CPU_Cores;
    unsigned int Shift = 0;
    while(Src[*Pos] && strncmp(Src + *Pos, "cpu", 3) == 0)
    {
        if(++Dest->Cores_Size > Dest->Cores_Capacity)
        {
            Reallocate_Cores(Dest);
            Cores_ptr = Dest->CPU_Cores + Shift;
        }
        Read_CPU_Stat(Cores_ptr, Src, Pos);
        Shift++;
        Cores_ptr = Dest->CPU_Cores + Shift;
    }
}

void Reallocate_Cores(CPU_Stats * Stats)
{
    Stats->Cores_Capacity *= ALLOC_MUL_COEFF;
    Stats->CPU_Cores = realloc(Stats->CPU_Cores, sizeof(CPU_Stat) * Stats->Cores_Capacity);
}