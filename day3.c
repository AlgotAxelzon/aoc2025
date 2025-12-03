#include <stdio.h>
#include <string.h>
#include <math.h>

int ascii_char_to_int(char c) {
    return c - '0';
}

int main() {
    FILE *f = fopen("inputs/3.txt", "r");

    char line[1000];

    unsigned long long int sum = 0;

    while (fgets(line, sizeof(line), f)) {
        int n_batteries = strlen(line) - 1;
        line[n_batteries] = '\0';
        printf("%s\n", line);

        unsigned long long int mini_sum = 0;

        int temp = 0;
        int index = -1;
        for (int k = 12; k > 0; k--) {
            temp = 0;
            //printf("index range: %i - %i\n", index+1, n_batteries-(k-1)-1);
            for (int i = index+1; i < n_batteries-(k-1); i++) {
                int jolt = ascii_char_to_int(line[i]);
                if (jolt > temp) {
                    temp = jolt;
                    index = i;
                }
            }
            //printf("temp: %u\n", temp);
            mini_sum += temp*(unsigned long long int)pow(10, k-1);
        }

        //// find the highest digit in range index 0..n-2
        //int bat1 = 0;
        //int bat1_index = 0;
        //for (int i = 0; i < n_batteries-1; i++) {
        //    int jolt = ascii_char_to_int(line[i]);
        //    if (jolt > bat1) {
        //        bat1 = jolt;
        //        bat1_index = i;
        //    }
        //}
        ////printf("bat1: %i, index: %i\n", bat1, bat1_index);

        //// find the highest digit in range index 0..n-2
        //int bat2 = 0;
        //int bat2_index = 0;
        //for (int i = bat1_index+1; i < n_batteries; i++) {
        //    int jolt = ascii_char_to_int(line[i]);
        //    if (jolt > bat2) {
        //        bat2 = jolt;
        //        bat2_index = i;
        //    }
        //}
        ////printf("bat2: %i, index: %i\n", bat2, bat2_index);

        //int jolt = bat1*10 + bat2;
        printf("jolt: %llu\n", mini_sum);

        sum += mini_sum;
    }

    printf("sum: %llu\n", sum);

    fclose(f);
}
