#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *read_file(FILE *f) {
    int index = 0;
    int capacity = 1;

    char *buf = NULL;

    do {
        if (index + 1 >= capacity) {
            capacity *= 2;
            buf = realloc(buf, capacity);
            if (buf == NULL) return NULL;
        }
        index = strlen(buf);
    } while (fgets(&buf[index], capacity-index, f));

    return buf;
}

char **read_lines(FILE *f, size_t *num_lines) {
    char *file = read_file(f);
    if (file == NULL) return NULL;

    char **lines = NULL;
    size_t line_count = 0;

    // Split the lines into different strings
    char *line;
    do {
        line = strsep(&file, "\n");

        // Add if "good" line :D 
        if (strlen(line) != 0 && file != NULL) {
            line_count++;
            lines = reallocarray(lines, line_count, sizeof(char *));
            lines[line_count-1] = line;
        }
    } while (file);

    *num_lines = line_count;
    return lines;
}

unsigned long int arr_sum(unsigned long int *list, size_t list_size) {
    unsigned long int sum = 0;

    for (int i = 0; i < list_size; i++) {
        sum += list[i];
    }
    return sum;
}

unsigned long int arr_product(unsigned long int *list, size_t list_size) {
    unsigned long int product = 1;

    for (int i = 0; i < list_size; i++) {
        product *= list[i];
    }
    return product;
}

unsigned long int sak(char c) {
    if ('0' <= c && c <= '9') {
        printf("digit %lu\n", (unsigned long int)(c - '0'));
        return (unsigned long int)(c - '0');
    }
    printf("digit 0\n");
    return 0;
}

int main() {
    FILE *f = fopen("inputs/6.txt", "r");

    size_t num_lines;
    char **lines = read_lines(f, &num_lines);

    unsigned long int sum = 0;

    size_t string_len = strlen(lines[0]);
    
    int stack_index = 0;
    unsigned long int stack[100];

    for (int col = string_len - 1; col >= 0; col--) {
        unsigned long int *number = &stack[stack_index];
        *number = 0;

        char c;
        int num_digits = 0;
        unsigned long digits[num_lines];
        for (int row = 0; row < num_lines; row++) {
            c = lines[row][col];
            if (row == num_lines-1) {
                for (unsigned int i = 0; i < num_digits; i++) {
                    printf("dig: %lu, pos: %i\n", digits[i], num_digits-1-i);
                    *number += digits[i]*(round(pow(10, num_digits-1-i)));
                }
                continue;
            }
            //printf("char: %c\n", c);
            if ('0' <= c && c <= '9') {
                unsigned long int s = c - '0';
                digits[(num_digits++)] = s;
            }
        }
        // check for operator
        if (c == '*') {
            printf("num: %lu\n", *number);
            unsigned long sub_sum = arr_product(stack, stack_index+1);
            printf("* sub_sum: %lu\n\n", sub_sum);
            sum += sub_sum;

            // reset number stack
            stack_index = 0;
            stack[stack_index] = 0;

            if (col != 0) col--;
        } else if (c == '+') {
            printf("num: %lu\n", *number);
            unsigned long sub_sum = arr_sum(stack, stack_index+1);
            printf("+ sub_sum: %lu\n\n", sub_sum);
            sum += sub_sum;

            // reset number stack
            stack_index = 0;
            stack[stack_index] = 0;
            if (col != 0) col--;
        } else {
            printf("num: %lu\n\n", *number);
            // push number
            stack_index++;
            stack[stack_index] = 0;
        }
    }

    printf("cum sum: %lu\n", sum);




    // // Use the last line to count the number of columns
    // size_t cols = 0;
    // char *last_line = lines[num_lines-1];
    // while (1) {
    //     last_line = strpbrk(last_line, "*+");
    //     if (last_line == NULL) break;

    //     // move past the match
    //     last_line++;

    //     cols++;
    // }
    // printf("cols = %li\n", cols);

    // size_t rows = num_lines-1;

    // // for storing all the numbers
    // unsigned long int numbers[cols][rows];

    // for (int i = 0; i < rows; i++) {
    //     char *l = lines[i];
    //     for (int j = 0; j < cols; j++) {
    //         numbers[j][i] = strtoul(l, &l, 10);
    //         printf("%lu ", numbers[j][i]);
    //     }
    //     printf("\n");
    //     
    //         

    //     //printf("%s\n", lines[i]);
    // }

    // unsigned long int sum = 0;

    // // Check the operation on last line
    // last_line = lines[num_lines-1];
    // for (int j = 0; j < cols; j++) {
    //     last_line = strpbrk(last_line, "*+");
    //     unsigned long int sub_sum;
    //     if (last_line[0] == '*') {
    //         sub_sum = arr_product(numbers[j], rows);
    //         printf("* ");
    //     } else if (last_line[0] == '+') {
    //         sub_sum = arr_sum(numbers[j], rows);
    //         printf("+ ");
    //     } else {
    //         printf("CP!!!");
    //         return -1;
    //     }

    //     printf("sub_sum: %lu\n", sub_sum);
    //     sum += sub_sum;
    //     last_line++;
    // }

    // printf("sum: %lu\n", sum);

    fclose(f);
}

int main_part1() {
    FILE *f = fopen("inputs/6.txt", "r");

    size_t num_lines;
    char **lines = read_lines(f, &num_lines);

    // Use the last line to count the number of columns
    size_t cols = 0;
    char *last_line = lines[num_lines-1];
    while (1) {
        last_line = strpbrk(last_line, "*+");
        if (last_line == NULL) break;

        // move past the match
        last_line++;

        cols++;
    }
    printf("cols = %li\n", cols);

    size_t rows = num_lines-1;

    // for storing all the numbers
    unsigned long int numbers[cols][rows];

    for (int i = 0; i < rows; i++) {
        char *l = lines[i];
        for (int j = 0; j < cols; j++) {
            numbers[j][i] = strtoul(l, &l, 10);
            printf("%lu ", numbers[j][i]);
        }
        printf("\n");
        
            

        //printf("%s\n", lines[i]);
    }

    unsigned long int sum = 0;

    // Check the operation on last line
    last_line = lines[num_lines-1];
    for (int j = 0; j < cols; j++) {
        last_line = strpbrk(last_line, "*+");
        unsigned long int sub_sum;
        if (last_line[0] == '*') {
            sub_sum = arr_product(numbers[j], rows);
            printf("* ");
        } else if (last_line[0] == '+') {
            sub_sum = arr_sum(numbers[j], rows);
            printf("+ ");
        } else {
            printf("CP!!!");
            return -1;
        }

        printf("sub_sum: %lu\n", sub_sum);
        sum += sub_sum;
        last_line++;
    }

    printf("sum: %lu\n", sum);

    fclose(f);
}
