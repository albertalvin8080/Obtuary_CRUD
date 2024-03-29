#include "obtuary.h"

size_t gDeadCount = 0;

void saveToDisk(Obtuary *obit)
{
	FILE *f;
	f = fopen("obtuary.bin", "wb");

	size_t salvos = fwrite(obit, sizeof(Obtuary), gDeadCount, f);
	if (salvos != gDeadCount)
	{
		printf("\n*ERROR: failed to persist data.\n");
		exit(EXIT_FAILURE);
	}

	fclose(f);
}

void unregisterDead(Obtuary *obit)
{
	int index;
	if (gDeadCount == 0)
	{
		printf("\nNOTHING TO UNREGISTER.\n");
	}
	else
	{
		printf("-------------------------------------------\n");
		printf("\t      EXCLUSION SCREEN\n");
		printf("-------------------------------------------");

		while (1)
		{
			printf("\nChoose and index between 0 and %zd to remove: ", gDeadCount - 1);
			scanf("%d", &index);
			cleanBuffer();

			if (index < 0 || index > gDeadCount - 1)
			{
				printf("\n*Invalid Index. Try again.");
			}
			else
			{
				break;
			}
		}

		for (int i = index; i < gDeadCount - 1; i++)
		{
			obit[i] = obit[i + 1];
		}
		gDeadCount--;

		printf("\nUNREGISTERED SUCCESSFULLY.\n");
	}
}

void registerDead(Obtuary *obit)
{
	if (gDeadCount >= MAX_DEAD)
	{
		printf("\n*WARNING: obtuary is full.\n");
	}
	else
	{
		printf("\n-------------------------------------------\n");
		printf("\t     REGISTER SCREEN\n");
		printf("-------------------------------------------");

		char confirm;
		do
		{
			printf("\nInsert the name: ");
			scanf("%64[^\n]", obit[gDeadCount].deadName);
			cleanBuffer();
			printf("Insert death cause: ");
			scanf("%128[^\n]", obit[gDeadCount].deathCause);
			cleanBuffer();
			printf("Insert description to the deceased: ");
			scanf("%128[^\n]", obit[gDeadCount].description);
			cleanBuffer();

			while (1)
			{
				printf("Insert birth date ->\nDay: ");
				scanf("%d", &obit[gDeadCount].birthDate.day);
				cleanBuffer();
				printf("Month: ");
				scanf("%d", &obit[gDeadCount].birthDate.month);
				cleanBuffer();
				printf("Year: ");
				scanf("%d", &obit[gDeadCount].birthDate.year);
				cleanBuffer();

				if (checkDates(
						obit[gDeadCount].birthDate.day,
						obit[gDeadCount].birthDate.month,
						obit[gDeadCount].birthDate.year) == 1)
				{
					printf("\n*Invalid date. Try again\n");
				}
				else
				{
					break;
				}
			}

			while (1)
			{
				printf("Insert date of death ->\nDay: ");
				scanf("%d", &obit[gDeadCount].deathDate.day);
				cleanBuffer();
				printf("Month: ");
				scanf("%d", &obit[gDeadCount].deathDate.month);
				cleanBuffer();
				printf("Year: ");
				scanf("%d", &obit[gDeadCount].deathDate.year);
				cleanBuffer();

				if (checkDates(
						obit[gDeadCount].deathDate.day,
						obit[gDeadCount].deathDate.month,
						obit[gDeadCount].deathDate.year) == 1)
				{
					printf("\n*Invalid date. Try again\n");
				}
				else if (compareDeath(obit) == 1)
				{
					printf("\n*The deceased cannot have died before it's birth.\n");
				}
				else
				{
					break;
				}
			}

			printf("\nAre you sure?\n(y/n): ");
			scanf("%c", &confirm);
		} while (confirm == 'n' || confirm == 'N');

		printf("\nREGISTERED SUCCESSFULLY.\n");

		gDeadCount++;
	}
}

int compareDeath(Obtuary *obit)
{
	if (obit[gDeadCount].deathDate.year < obit[gDeadCount].birthDate.year)
	{
		return 1;
	}
	else if (
		obit[gDeadCount].deathDate.year == obit[gDeadCount].birthDate.year && obit[gDeadCount].deathDate.month < obit[gDeadCount].birthDate.month)
	{
		return 1;
	}
	else if (
		obit[gDeadCount].deathDate.year == obit[gDeadCount].birthDate.year && obit[gDeadCount].deathDate.month == obit[gDeadCount].birthDate.month && obit[gDeadCount].deathDate.day < obit[gDeadCount].birthDate.day)
	{
		return 1;
	}

	return 0;
}

int checkDates(int day, int month, int year)
{
	if (day < 1 || day > 31)
	{
		return 1;
	}
	else if (month < 1 || month > 12)
	{
		return 1;
	}
	else if (day > 28 && month == 2)
	{
		return 1;
	}
	else if (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11))
	{
		return 1;
	}

	return 0;
}

void listAll(Obtuary *obit)
{
	if (gDeadCount == 0)
	{
		printf("\nNO DATA AT THE MOMENT.\n");
	}
	else
	{
		printf("\n-------------------------------------------\n");
		printf("\t     LIST ALL SCREEN\n");
		printf("-------------------------------------------\n");
		for (int i = 0; i < gDeadCount; i++)
		{
			printf("\n|Deceased %d|\n", i);
			printf("- Name: %s\n", obit[i].deadName);
			printf("- Birth date: %d/%d/%d\n", obit[i].birthDate.day,
				   obit[i].birthDate.month, obit[i].birthDate.year);
			printf("- Death date: %d/%d/%d\n", obit[i].deathDate.day,
				   obit[i].deathDate.month, obit[i].deathDate.year);
			printf("- Death cause: %s\n", obit[i].deathCause);
			printf("- Description: %s\n", obit[i].description);
		}
	}
}

void loadRecordsToProgramMemory(Obtuary *obit)
{
	FILE *f;
	f = fopen("obtuary.bin", "rb");
	if (f == NULL)
	{
		printf("*Failed to open file.\n(Remember to create 'obtuary.bin' manually)\n");
		exit(EXIT_FAILURE);
	}

	gDeadCount = fread(obit, sizeof(Obtuary), MAX_DEAD, f);
	printf("Loaded Data = %zd\n", gDeadCount);

	if (gDeadCount >= MAX_DEAD)
	{
		printf("*WARNING: obtuary is full.\n");
	}

	fclose(f);
}

void cleanBuffer(void)
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}
