#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

int read_until_char(char until_c, FILE *f, char* buf) {
    while (1) {
        int c = fgetc(f);
        if (c == EOF) {
            *buf = '\0';
            return EOF;
        } else if (c == until_c) {
            *buf = '\0';
            return 0;
        } else {
            *buf = (char)c;
        }
        buf++;
    }
}

bool is_invalid_id(unsigned long long int id) {
    static char buf[1000];

    // Put the shit in a string
    int n = sprintf(buf, "%llu", id);

    if (id > (unsigned long long int)pow(10,n)) {
        fprintf(stderr, "bigger\n");
        fprintf(stderr, "id='%llu' does not have n=%i digits!\n", id, n);
        while (1) {}
    }

    if (id < (unsigned long long int)pow(10,n-1)) {
        fprintf(stderr, "smaller\n");
        fprintf(stderr, "id='%llu' does not have n=%i digits!\n", id, n);
        while (1) {}
    }

    if (n == 1) return false;
    //printf("n=%i\n", n);

    // Test patterns of length 1, 2, 3, .. etc
    for (int pattern_len = 1; pattern_len <= (n/2); pattern_len++) {
    //printf("half_n:%i", pattern_len);
        // the pattern len is only working if it is a multiple of the string len..
        if (n % pattern_len != 0) continue;

        // set the pattern
        //strncpy(pattern_buf, buf, pattern_len);
        //pattern_buf[pattern_len] = '\0';

        bool pattern_match = true;
        for (int i = 1; i < (n/pattern_len); i++) {
            if (0 != strncmp(buf, &buf[i*pattern_len], pattern_len)) {
                pattern_match = false;
                //break;
            }
        }

        if (pattern_match) {
            // it is invalid!
            printf("%llu, %i\n", id, pattern_len);
            return true;
        }
    }

    return false;
}

int main() {
    FILE *f = fopen("inputs/2.txt", "r");
    char input[10000];

    fgets(input, sizeof(input), f);

    printf("%s", input);

    char *token = NULL;

    unsigned long long int sum = 0;

    while (1) {
        if (token == NULL) {
            token = strtok(input, ",");
        } else {
            token = strtok(NULL, ",");
        }
        if (token == NULL) break;

        unsigned long long int low, high;
        int s = sscanf(token, "%llu-%llu", &low, &high);

        for (unsigned long long int id = low; id <= high; id++) {
            if (is_invalid_id(id)) {
                sum += id;
            }
        }
    }

    printf("sum: %llu\n", sum);

    fclose(f);
    return 0;
}
/*
    int sak;
    do {
        sak = read_until_char(',', f, buf);

        uint64_t low;
        uint64_t high;
        int s = sscanf(buf, "%lu-%lu", &low, &high);
        if (s == 2) {
            printf("%lu to %lu\n", low, high);


            uint64_t part_sum = 0;
            for (uint64_t i = low; i <= high; i++) {
                //int n = sprintf(buf, "%lu", i);
                if(is_invalid_id(i)) {
                    //printf("invlid id: %u\n", i);

                    part_sum += i;
                }
            }
            printf("part sum %lu\n", part_sum);
            final_sum += part_sum;
            //printf("updated final: %lu\n", final_sum);
        }

    } while (sak != EOF);

    // // Try to get everything
    // if(NULL == fgets(line, sizeof(line), f)) {
    //     sprintf(stderr, "Error reading file\n");
    //     return -1;
    // }

    // // try a scan
    // unsigned int low;
    // unsigned int high;
    // int n = sscanf(line, "%u-%u", &low, &high);

    fclose(f);

    printf("final sum: %lu\n", final_sum);
}
*/
