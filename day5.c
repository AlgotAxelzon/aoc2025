#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include <stdlib.h>

struct Range {
    unsigned long long int low;
    unsigned long long int high;
};

struct RangeStack {
    struct Range *data;
    int items;
    int capacity;
};

void print_stack(struct RangeStack s) {
    printf("stack: {.data = %p, .items = %i, .capacity = %i}\n", s.data, s.items, s.capacity);
}
void print_range(struct Range r) {
    printf("range: {.low = %li, .high = %li}\n", r.low, r.high);
}


int pop(struct RangeStack *stack, struct Range *r) {
    // TODO: shrink the data
    if (stack->items > 0) {
        *r = ((struct Range *)stack->data)[--stack->items];
        return 0;
    } else {
        return -1;
    }
}

int push(struct RangeStack *stack, struct Range range) {
    printf("pushing: ");
    print_range(range);
    if (stack->items + 1 > stack->capacity) {
        void *d = realloc(stack->data, sizeof(struct Range)*(stack->capacity*2));
        if (d == NULL) return -1;
        stack->data = d;
        stack->capacity = stack->capacity * 2;
    }
    ((struct Range *)stack->data)[stack->items++] = range;
    return 0;
}

struct RangeStack init_stack() {
    void *data = malloc(sizeof(struct Range));
    struct RangeStack stack = { .data = data, .items = 0, .capacity = 1 };
    return stack;
}

int main3() {
    printf("Testing my stack!\n");
    
    struct RangeStack s = init_stack();
    print_stack(s);

    struct Range r1 = { .low = 4, .high = 7 };
    push(&s, r1);
    print_stack(s);

    struct Range r2 = { .low = 10, .high = 33 };
    push(&s, r2);
    print_stack(s);

    push(&s, r1);
    print_stack(s);



    int ret;
    struct Range range;
    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);

    ret = pop(&s, &range);
    print_range(range);


    print_stack(s);

    return 0;
}


int main() {
	FILE *f = fopen("inputs/5.txt", "r");

    unsigned long long int ranges[1000][2];

    char line[100];

    int ret;

    // Collect all the ranges
    unsigned long long int num_ranges = 0;

    // stack of ranges
    struct RangeStack stack = init_stack();

    while(fgets(line, sizeof(line), f)) {
        // Break if blank line
	    if (strlen(line) == 1) break;

        unsigned long long int low = 0, high = 0;
        int n = sscanf(line, "%llu-%llu\n", &low, &high);
        //printf("%lu - %lu\n", low, high);

        printf("\nread: %llu-%llu\n", low, high);

        // add this shit to the stack
        struct Range r = {.low = low, .high = high};
        ret = push(&stack, r);
        if (ret) {
            fprintf(stderr, "push fail\n");
            return -1;
        }

        printf("tja\n");

        // Loop until stack is empty
        do {
            printf("current ranges:\n");
            for (int r = 0; r < num_ranges; r++) {
                printf("%i: %llu - %llu\n", r, ranges[r][0], ranges[r][1]);
            }
            printf("\n");

            printf("poping stack\n");

            struct Range r;
            ret = pop(&stack, &r);
            print_range(r);

            low = r.low;
            high = r.high;

            if (ret) {
                fprintf(stderr, "pop fail\n");
                return -1;
            }

            bool handled = false;

            for (int r = 0; r < num_ranges; r++) {
                printf("comparing against: %llu - %llu\n", ranges[r][0], ranges[r][1]);
                
                // cases
                // 0 already exists
                if (low == ranges[r][0] && high == ranges[r][1]) {
                    handled = true;
                    break;
                }
                // 1. totaly overlap -> push 2 side ranges to stack
                if (low < ranges[r][0] && high > ranges[r][1]) {
                    printf("case1\n");
                    struct Range lower = {.low = low, .high = ranges[r][0]-1};
                    struct Range upper = {.low = ranges[r][1]+1, .high = high};

                    ret = push(&stack, lower);
                    if (ret) {
                        fprintf(stderr, "push fail\n");
                        return -1;
                    }
                    ret = push(&stack, upper);
                    if (ret) {
                        fprintf(stderr, "push fail\n");
                        return -1;
                    }
                    handled = true;
                    break;
                }

                // 2. lower overlapping -> push new range to stack 
                if (low < ranges[r][0] && high <= ranges[r][1] && high >= ranges[r][0]) {
                    printf("case2\n");
                    struct Range lower = {.low = low, .high = ranges[r][0]-1};
                    ret = push(&stack, lower);
                    if (ret) {
                        fprintf(stderr, "push fail\n");
                        return -1;
                    }
                    handled = true;
                    break;
                }

                // 3. upper overlapping -> push new range to stack 
                if (low >= ranges[r][0] && low <= ranges[r][1] && high > ranges[r][1]) {
                    printf("case3\n");
                    struct Range upper = {.low = ranges[r][1]+1, .high = high};
                    ret = push(&stack, upper);
                    if (ret) {
                        fprintf(stderr, "push fail\n");
                        return -1;
                    }
                    handled = true;
                    break;
                }

                // 4. inside overlap -> nothing
                if (low >= ranges[r][0] && high <= ranges[r][1]) {
                    printf("case4\n");
                    handled = true;
                    break;
                }

                if (handled)
                    break;

                

            }

            if (!handled) {
                printf("adding range: %llu - %llu\n", low, high);
                ranges[num_ranges][0] = low;
                ranges[num_ranges][1] = high;
                num_ranges++;
            }
            //sleep(1);
        } while (stack.items > 0);
    }

    printf("num_ranges: %llu\n", num_ranges);

    unsigned long long int sum = 0;
    for (int r = 0; r < num_ranges; r++) {
        if (ranges[r][0] > ranges[r][1]) {
            printf("cp");
            return -1;
        }
        printf("%i: %llu - %llu\n", r, ranges[r][0], ranges[r][1]);
        unsigned long long int sak = (ranges[r][1] - ranges[r][0])+1;
        printf("sak: %llu\n\n", sak);
        sum += sak;
    }

            printf("current ranges:\n");
            for (int r = 0; r < num_ranges; r++) {
                printf("%i: %llu - %llu\n", r, ranges[r][0], ranges[r][1]);
            }

    printf("sum: %llu\n", sum);

    // long int num_fresh = 0;


    // for (long int r = 0; r < num_ranges; r++) {
    //     printf("range number: %lu  (%lu-%lu)\n", r, ranges[r][0], ranges[r][1]);
    //     // check all numbers in the range
    //     for (unsigned long number = ranges[r][0]; number <= ranges[r][1]; number++) {
    //         num_fresh++;
    //         //printf("fresh: %lu\n", number);
    //         for (unsigned long int r_under = 0; r_under < r; r_under++) {
    //             //printf("%lu\n", r_under);
    //             if (r == 0) break;
    //             // Check if it has already been counted
    //             if (ranges[r_under][0] <= number && number <= ranges[r_under][1]) {
    //                 num_fresh--;
    //                 //printf("actually already counted: %lu\n", number);
    //                 break;
    //             }
    //         }
    //     }
    // }
    // printf("num_fresh: %li\n", num_fresh);

    // for (long int i = min; i <= max; i++) {
    //     bool in_range = false;
    //     for (long int r = 0; r < num_ranges; r++) {
    //         if (ranges[r][0] <= i && i <= ranges[r][1]) {
    //             in_range = true;
    //             break;
    //         }
    //     }
    //     if (in_range) {
    //         //printf("f: %li\n", i);
    //         num_fresh++;
    //     }

    // }


    // int num_fresh = 0;

    // // Get the numbers
    // long int number;
    // while(fgets(line, sizeof(line), f)) {
    //     int n = sscanf(line, "%li\n", &number);
    //     //printf("num: %i\n", number);

    //     bool in_range = false;
    //     for (int r = 0; r < num_ranges; r++) {
    //         if (ranges[r][0] <= number && number <= ranges[r][1]) {
    //             in_range = true;
    //             break;
    //         }
    //     }

    //     if (in_range) {
    //         num_fresh++;
    //     }

    // }

    // printf("num_fresh: %i\n", num_fresh);

	fclose(f);

	return 0;
}
