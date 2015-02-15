#include <stdio.h>
#define ROW 3
#define COL 3
int main(int argc, char *argv[])
{
    int i, j;
    char *matrix[ROW][COL] = {
        {"aa", "bb", "cc"},
        {"dd", "ee", "ff"},
        {"gg", "hh", "ii"},
    };

    for(i = 0; i < ROW; i++){
        for(j = 0; j < COL; j++){
            printf("matrix[%d][%d] is %s\n", i, j, matrix[i][j]);
        }
    }

    return 0;
}
