#ifndef OBTUARY_H
#define OBTUARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_DEAD 1000

// Global Index - dead count
extern size_t gDeadCount;

typedef struct
{
	int day, month, year;
} Date;

typedef struct
{
	Date deathDate;
	Date birthDate;
	char deadName[64];
	char deathCause[128];
	char description[128];
} Obtuary;

typedef enum
{
	MAIN_SCREEN = -1,
	OUT_SCREEN,	   // 0
	REGISTER_SCREEN,   // 1
	UNREGISTER_SCREEN, // 2
	LIST_ALL_SCREEN	   // 3
} Screen;

void cleanBuffer(void);
void listAll(Obtuary *obit);
void saveToDisk(Obtuary *obit);
void registerDead(Obtuary *obit);
int compareDeath(Obtuary *obit);
void unregisterDead(Obtuary *obit);
int checkDates(int day, int month, int year);
void loadRecordsToProgramMemory(Obtuary *obit);

#endif