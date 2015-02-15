#include <stdio.h>
#include "../french_number.h"

#define MINUTES 60

int main(int argc, char *argv[])
{
    int m;

    for(m = 0; m < MINUTES; m++){
       printf("%d minutes = %s %s\n", m, french_number_60[m][0], french_number_60[m][1]);
    }

    return 0;
}
