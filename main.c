#include <stdio.h>
#include <string.h>

void put_file(const char *path);
void print_schema(void);
void print_stats(void);

static void print_usage(FILE *out){
    fprintf(out, "Usage:\n");
    fprintf(out, "  csvstat --help\n");
    fprintf(out, "  csvstat schema <csv-file>\n");
    fprintf(out, "  csvstat stats <csv-file>\n");
}

int main(int argc, char *argv[]){
    if (argc < 2){
        print_usage(stderr);
        return 2;
    }
    if (strcmp(argv[1], "--help") == 0){
        if (argc != 2){
            print_usage(stderr);
            return 2;
        }
        print_usage(stdout);
        return 0;
    }
    int is_schema = (strcmp(argv[1], "schema") == 0);
    int is_stats = (strcmp(argv[1], "stats") == 0);
    if ((!is_schema && !is_stats) || argc != 3){
        print_usage(stderr);
        return 2;
    }
    put_file(argv[2]);
    if (is_schema){
        print_schema();
    }
    else{
        print_stats();
    }
    return 0;
}
