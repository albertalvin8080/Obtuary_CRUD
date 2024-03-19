#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "obtuary.h"

int main()
{
	// setlocale(LC_ALL,"Portuguese_Brazil");
	// printf("%zu\n", gDeadCount);

	Screen menu;
	Obtuary *obit = (Obtuary *)calloc(MAX_DEAD, sizeof(Obtuary));
	// Obtuary *obit = (Obtuary *)malloc(MAX_DEAD * sizeof(Obtuary));
	// memset(obit, 0, MAX_DEAD * sizeof(Obtuary));

	loadRecordsToProgramMemory(obit);

	printf("===========================================\n");
	printf("\t\tOBTUARY\n");
	printf("===========================================\n\n");

	do
	{
		printf("-------------------------------------------\n");
		printf("\t      MAIN SCREEN\n");
		printf("-------------------------------------------\n");
		printf("\t-MENU-\n\t");
		printf("1 -> Register;\n\t");
		printf("2 -> Unregister;\n\t");
		printf("3 -> List all;\n\t");
		printf("0 -> End session.\n\t?");
		scanf("%d", &menu);
		cleanBuffer();

#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif

		switch (menu)
		{
		case REGISTER_SCREEN:
		{
			registerDead(obit);
			menu = MAIN_SCREEN;
		}
		break;

		case UNREGISTER_SCREEN:
		{
			unregisterDead(obit);
			menu = MAIN_SCREEN;
		}
		break;

		case LIST_ALL_SCREEN:
		{
			listAll(obit);
			menu = MAIN_SCREEN;
		}
		break;

		case OUT_SCREEN:
		{
			saveToDisk(obit);
			printf("===========================================\n");
			printf("\t\t  END\n");
			printf("===========================================");
		}
		break;

		default:
		{
			printf("*Invalid Option\n");
			menu = MAIN_SCREEN;
		}
		break;
		}
		printf("\n");

	} while (menu == MAIN_SCREEN);

	free(obit);

	return (EXIT_SUCCESS);
}

