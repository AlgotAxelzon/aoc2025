#include <stdio.h>

int main() {
    FILE *f = fopen("inputs/1.txt", "r");

    int val = 50;

    int times_zero = 0;

    char line[100];

    while (fgets(line, sizeof(line), f)) {
        // First char should be either 'L' or 'R'
        int sign = line[0] == 'R' ? 1 : -1;

        int skräp;
        int n;
        if(sscanf(line+1, "%u\n", &n)) {
            // Check if we have wrapped or is at 0

            for (int i = 0; i < n; i++) {
                val = val + sign;
                if (val % 100 == 0) {
                    times_zero++;
                }
            }

        }
    }

    printf("times zero: %i\n", times_zero);



    fclose(f);
}
