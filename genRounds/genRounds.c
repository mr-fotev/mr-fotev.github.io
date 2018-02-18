#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

// тези масиви са за статистиката
int STATS[N][N];
int LEVELS[N];

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

// структура, която поддържа играта
typedef struct tGame
{
	int players;
	int tables;
	int roundNumber;

	int* seats;
} Game;

// инициализация на игра с брой играчи и брой маси
int initGame(Game* g, int players, int tables)
{
	if (players < 3 * tables)
	{
		printf("Not enough players!");
		return -1;
	}
	else if (players > 4 * tables)
	{
		printf("Not enough tables!");
		return -1;
	}

	g->players = players;
	g->tables = tables;

	g->roundNumber = 1;

	int i, k;

	g->seats = (int*) malloc(players * sizeof(*(g->seats)));

	// генерира начално разпределение на масите... гарантирано е, че на всяка ще има поне по 3 и най-много 4 човека
	for (i = 0, k = 0; i < players; i++)
	{
		g->seats[i] = k;
		k = (k + 1) % tables;
	}

	return 0;
}

// принти следващия кръг
void printNextRound(Game* g)
{
	int players = g->players, tables = g->tables;

	int i, k;

	// генерира ново random разпределение
	for (i = players - 1; i > 0; i--)
	{
		k = rand() % i;
		swap(&g->seats[i], &g->seats[k]);
	}

	/*
		"Поправя" разпределението за 1000 стъпки. Как прави поправката? Избира случаен играч X и намира:
	- играч A - играч в същата група, с когото X е играл най-много пъти.
	- играч B - играч, с когото X е играл най-малко пъти изобщо (не само от тази група).
		Играчите A и B си разменят местата.
	*/

	int j;
	for (j = 0; j < 1000; j++)
	{
		int rPlayer = rand() % players;

		int maxGamesSameGroup = -1;
		int maxPlayerSameGroup = 0;

		int minGames = N;
		int minPlayer = 0;

		for (i = 0; i < players; i++) if (i != rPlayer)
		{
			if (g->seats[rPlayer] == g->seats[i] && STATS[rPlayer][i] > maxGamesSameGroup)
			{
				maxGamesSameGroup = STATS[rPlayer][i];
				maxPlayerSameGroup = i;
			}
			
			if (STATS[rPlayer][i] < minGames)
			{
				minGames = STATS[rPlayer][i];
				minPlayer = i;
			}
		}

		swap(&g->seats[maxPlayerSameGroup], &g->seats[minPlayer]);
	}

	// принти разпределението
	printf("Round %d:\n", g->roundNumber);
	for (i = 0; i < players; i++) printf("%d ", g->seats[i]);
	printf("\n\n");

	g->roundNumber++;

	// update-ва статистиката
	for (i = 0; i < players - 1; i++) for (k = i + 1; k < players; k++)
	{
		if (g->seats[i] == g->seats[k])
		{
			STATS[i][k]++;
			STATS[k][i]++;
		}
	}
}

int main()
{
	// инициализира помощните масиви
	int i, j;
	for (i = 0; i < N; i++)
	{
		LEVELS[i] = 0;
		for (j = 0; j < N; j++) STATS[i][j] = 0;
	}

	// примерна игра за 40 човека на 10 маси и 20 кръга
	int Players = 40;
	int Tables = 10;
	int Rounds = 20;

	Game newGame;
	initGame(&newGame, Players, Tables);

	srand((unsigned int)time(0));

	printf("A schedule for %d players on %d tables and %d rounds\n\n", Players, Tables, Rounds);

	// принти разпределението за кръговете
	for (i = 0; i < Rounds; i++) printNextRound(&newGame);

	// събира информация за статистиката
	float avgN = 0;
	int maxLEVEL = 0;

	for (i = 0; i < Players - 1; i++)
	{
		for (j = i + 1; j < Players; j++)
		{
			LEVELS[STATS[i][j]]++;
			if (STATS[i][j] > maxLEVEL) maxLEVEL = STATS[i][j];
		}
	}

	avgN = (LEVELS[0] * 2.0f) / Players;

	printf("Every player played with average %.3f%% of all other players\n\n", (Players - 1 - avgN) * 100.0f / (Players - 1));

	printf("Now printing pairs statistics:\n");
	int P = Players * (Players - 1);
	for (i = 0; i <= maxLEVEL; i++)
	{
		printf("%d: %d", i, LEVELS[i]);
		printf("\t(%.3f%% of all pairs)\n", LEVELS[i] * 200.0f / P);
	}

	printf("\n");
	
	return 0;
}
