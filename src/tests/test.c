
#include <stdio.h>

int round_to_5(int min) {
	return ((min+2)/5)*5;
}

void main() {
	int i;
	
	for (i = 0; i < 31; i++){
		printf ("round %d to five = %d\n", i, round_to_5(i));
	}
}