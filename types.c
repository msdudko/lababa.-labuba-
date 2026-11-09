#include <stdlib.h>
#include <math.h>
#define T_NULL 0
#define T_INT 1
#define T_DOUBLE 2
#define T_STRING 3

extern int nrows;
char *get_cell(int r, int c);
int check_int(const char *s, long long *znach){
    int znak = 0;
    if (*s == '+' || *s == '-'){
        if (*s == '-'){
            znak = 1;
        }
        s++;
    }
    if (*s == '\0'){
        return 0;
    }
    unsigned long long mmax = 9223372036854775807ULL;
    if (znak){
        mmax = 9223372036854775808ULL;
    }
    unsigned long long number = 0;
    while (*s != '\0'){
        if (*s < '0' || *s > '9'){
            return 0;
        }
        unsigned long long digit = *s - '0';
        if (number > (mmax - digit) / 10){
            return 0;
        }
        number = number * 10 + digit;
        s++;
    }
    if (znak){
        if (number == 9223372036854775808ULL){
            *znach = -9223372036854775807LL - 1;
        }
        else{
            *znach = -(long long)number;
        }
    }
    else{
        *znach = (long long)number;
    }
    return 1;
}

static int check_double(const char *s){
    if (*s == '+' || *s == '-'){
        s++;
    }
    int dot = 0;
    while (*s >= '0' && *s <= '9'){
        s++;
        dot++;
    }
    if (dot == 0 || *s != '.'){
        return 0;
    }
    s++;
    int posle = 0;
    while (*s >= '0' && *s <= '9'){
        s++;
        posle++;
    }
    if (posle == 0 || *s != '\0'){
        return 0;
    }
    return 1;
}

static int get_type(const char *s){
    long long x;
    if (s == NULL){
        return T_NULL;
    }
    if (check_int(s, &x)){
        return T_INT;
    }
    if (check_double(s)){
        double d = strtod(s, NULL);
        if (isfinite(d)){
            return T_DOUBLE;
        }
    }
    return T_STRING;
}

int column_type(int c){
    int type = T_NULL;
    for (int r = 0; r < nrows; r++){
        int t = get_type(get_cell(r, c));
        if (t > type){
            type = t;
        }
    }
    return type;
}

const char *type_name(int type){
    if (type == T_NULL){
        return "NULL";
    }
    if (type == T_INT){
        return "INT";
    }
    if (type == T_DOUBLE){
        return "DOUBLE";
    }
    return "STRING";
}
