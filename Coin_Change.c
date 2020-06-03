//
#include <stdio.h>

char coin_names[5][256] = {
	"half_dollar",
	"quarter",
	"dime",
	"nickel",
	"penny"
};
int coin_prices[] = {50, 25, 10, 5, 1};
const int COINT_TYPE_CNT = 5;

int FindCoins(int cents, int* coins_needed);
void PrintCoinsNeeded(int* coins_needed);

int main(void) {
	int cents;
	printf("Enter money in cents: ");
	scanf("%d", &cents);

	int coins_needed[COINT_TYPE_CNT];
	int totalCoinsCnt = FindCoins(cents, coins_needed);

	printf("%d cents =\n", cents);
	PrintCoinsNeeded(coins_needed);

	printf("Total number of coins: %d\n", totalCoinsCnt);

	return 0;
}

int FindCoins(int cents, int* coins_needed){
	int remainder = cents;
	for(int n=0; n<COINT_TYPE_CNT; n++){
		coins_needed[n] = remainder / coin_prices[n];
		remainder = remainder % coin_prices[n];
	}
	int sum = 0;
	for(int i=0; i<COINT_TYPE_CNT; i++)
		sum += coins_needed[i];
	return sum;
}

void PrintCoinsNeeded(int* coins_needed){
	printf("%d(%s)", coins_needed[0], coin_names[0]);
	for (int i=1; i<COINT_TYPE_CNT; i++)
		printf(" + %d(%s)", coins_needed[i], coin_names[i]);
	printf("\n");
}
