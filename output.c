#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define T_NULL 0
#define T_INT 1
#define T_DOUBLE 2
#define T_STRING 3

extern char **cells;
extern int ncols;
extern int nrows;
char *get_cell(int r, int c);
int check_int(const char *s, long long *value);
int column_type(int c);
const char *type_name(int type);

static void print_name(const char *name){
    putchar('"');
    for (int i = 0; name[i] != '\0'; i++){
        if (name[i] == '"'){
            putchar('"');
        }
        putchar(name[i]);
    }
    putchar('"');
}

static void print_double(long double x) {
    if (x == 0.0L) {
        printf("0.000000");
    } else {
        printf("%.6Lf", x);
    }
}

void print_schema(void){
    printf("rows %d\n", nrows);
    printf("columns %d\n", ncols);
    for (int c = 0; c < ncols; c++){
        printf("%d ", c);
        print_name(cells[c]);
        printf(" %s\n", type_name(column_type(c)));
    }
}

static void print_int_stats(int c){
    int nulls = 0;
    int count = 0;
    int positive = 0;
    int negative = 0;
    int even = 0;
    long long min = 0, max = 0;
    long double sum = 0;
    for (int r = 0; r < nrows; r++){
        char *cell = get_cell(r, c);
        if (cell == NULL){
            nulls++;
            continue;
        }
        long long x = 0;
        check_int(cell, &x);
        if (count == 0 || x < min){
            min = x;
        }
        if (count == 0 || x > max){
            max = x;
        }
        if (x > 0){
            positive++;
        }
        if (x < 0){
            negative++;
        }
        if (x % 2 == 0){
            even++;
        }
        sum += x;
        count++;
    }

    printf("nulls %d\n", nulls);
    printf("min %lld\n", min);
    printf("max %lld\n", max);
    printf("mean ");
    print_double(sum / count);
    printf("\npositive %d\n", positive);
    printf("negative %d\n", negative);
    printf("even %d\n", even);
}

static void print_double_stats(int c){
    int nulls = 0;
    int count = 0;
    int positive = 0;
    int negative = 0;
    int integer = 0;
    double min = 0, max = 0;
    long double sum = 0;
    for (int r = 0; r < nrows; r++){
        char *cell = get_cell(r, c);
        if (cell == NULL){
            nulls++;
            continue;
        }
        double x = strtod(cell, NULL);
        if (count == 0 || x < min){
            min = x;
        }
        if (count == 0 || x > max){
            max = x;
        }
        if (x > 0){
            positive++;
        }
        if (x < 0){
            negative++;
        }
        if (x == floor(x)){
            integer++;
        }
        sum += x;
        count++;
    }

    printf("nulls %d\n", nulls);
    printf("min ");
    print_double(min);
    printf("\nmax ");
    print_double(max);
    printf("\nmean ");
    print_double(sum / count);
    printf("\npositive %d\n", positive);
    printf("negative %d\n", negative);
    printf("integer %d\n", integer);
}

static void print_string_stats(int c){
    int nulls = 0;
    int count = 0;
    int empty = 0;
    int min = 0;
    int max = 0;
    long long sum = 0;
    for (int r = 0; r < nrows; r++){
        char *cell = get_cell(r, c);
        if (cell == NULL){
            nulls++;
            continue;
        }
        int len = strlen(cell);
        if (count == 0 || len < min){
            min = len;
        }
        if (count == 0 || len > max){
            max = len;
        }
        if (len == 0){
            empty++;
        }
        sum += len;
        count++;
    }
    printf("nulls %d\n", nulls);
    printf("min_length %d\n", min);
    printf("max_length %d\n", max);
    printf("mean_length ");
    print_double((long double)sum / count);
    printf("\nempty %d\n", empty);
}

void print_stats(void){
    printf("rows %d\n", nrows);
    printf("columns %d\n", ncols);
    for (int c = 0; c < ncols; c++){
        int type = column_type(c);
        printf("\ncolumn %d\n", c);
        printf("name ");
        print_name(cells[c]);
        printf("\ntype %s\n", type_name(type));
        if (type == T_NULL){
            printf("rows %d\n", nrows);
        }
        else if (type == T_INT){
            print_int_stats(c);
        }
        else if (type == T_DOUBLE){
            print_double_stats(c);
        }
        else{
            print_string_stats(c);
        }
    }
}
