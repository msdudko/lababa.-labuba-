#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **cells = NULL;
int ncols = 0;
int nrows = 0;

static int cells_count = 0; 
static int cells_cap = 0; 

void oshibka(const char *gr, int ln){
    if (ln > 0){
        fprintf(stderr, "error: %s (line %d)\n", gr, ln);
    }
    else{
        fprintf(stderr, "error: %s\n", gr);
    }
    exit(1);
}

char *get_cell(int r, int c){
    return cells[(r + 1) * ncols + c];
}

static char *read_file(const char *p, int *len){
    FILE *f = fopen(p, "rb");
    if (f == NULL){
        oshibka("not open file", 0);
    }
    int size = 1;
    int n = 0;
    char *text = malloc(size);
    if (text == NULL){
        oshibka("out of memory", 0);
    }
    int c;
    while ((c = fgetc(f)) != EOF){
        text[n] = c;
        n++;
        if (n == size){
            size *= 2;
            char *tmp = realloc(text, size);
            if (tmp == NULL){
                oshibka("out of memory", 0);
            }
            text = tmp;
        }
    }
    fclose(f);
    *len = n;
    return text;
}

static void add_cell(char *value){
    if (cells_count == cells_cap){
        if (cells_cap == 0){
            cells_cap = 64;
        }
        else{
            cells_cap *= 2;
        }
        char **tmp = realloc(cells, cells_cap * sizeof(char *));
        if (tmp == NULL){
            oshibka("out of memory", 0);
        }
        cells = tmp;
    }
    cells[cells_count] = value;
    cells_count++;
}

static char *make_copy(const char *s, int n){
    char *p = malloc(n + 1);
    if (p == NULL){
        oshibka("out of memory", 0);
    }
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

static int is_space(char c){
    return c == ' ' || c == '\t';
}

static int parse_line(const char *s, int len, int ln){
    int count = 0;
    int i = 0;
    char *buf = malloc(len + 1);
    if (buf == NULL){
        oshibka("out of memory", 0);
    }
    while (1){
        while (i < len && is_space(s[i])){
            i++;
        }
        if (i < len && s[i] == '"'){
            i++;
            int n = 0;
            int closed = 0;
            while (i < len){
                if (s[i] == '"' && i + 1 < len && s[i + 1] == '"'){
                    buf[n] = '"';
                    n++;
                    i += 2;
                } 
                else if (s[i] == '"'){
                    i++;
                    closed = 1;
                    break;
                }
                else{
                    buf[n] = s[i];
                    n++;
                    i++;
                }
            }
            if (closed == 0){
                oshibka("unclosed quote", ln);
            }
            add_cell(make_copy(buf, n));
            while (i < len && is_space(s[i])){
                i++;
            }
            if (i < len && s[i] != ','){
                oshibka("symbols after closing quote", ln);
            }
        } 
        else{
            int start = i;
            while (i < len && s[i] != ','){
                if (s[i] == '"'){
                    oshibka("quote inside simple field", ln);
                }
                i++;
            }
            int end = i;
            while (end > start && is_space(s[end - 1])){
                end--;
            }
            if (end == start){
                add_cell(NULL);    
            }
            else{
                add_cell(make_copy(s + start, end - start));
            }
        }
        count++;
        if (i >= len){
            break;
        }
        i++;
    }
    free(buf);
    return count;
}

void put_file(const char *p){
    int len = 0;
    char *text = read_file(p, &len);
    if (len == 0){
        oshibka("empty file", 0);
    }
    while (len > 0 && text[len - 1] == '\n'){
        len--;
    }
    int pos = 0;
    int ln = 1;  
    while(1){
        int end = pos;
        while (end < len && text[end] != '\n'){
            end++;
        }
        int n = parse_line(text + pos, end - pos, ln);
        if (ln == 1){
            ncols = n;
            for (int i = 0; i < ncols; i++){
                if (cells[i] == NULL || cells[i][0] == '\0'){
                    oshibka("empty column name", ln);
                }
                for (int j = 0; j < i; j++){
                    if (strcmp(cells[i], cells[j]) == 0){
                        oshibka("duplicate column name", ln);
                    }
                }
            }
        }
        else{
            if (n != ncols){
                oshibka("wrong number of fields", ln);
            }
            nrows++;
        }
        if (end >= len){
            break;
        }
        pos = end + 1;
        ln++;
    }
    free(text);
}
