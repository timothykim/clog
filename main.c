#include <stdio.h>
#include <stdlib.h>
#include "fcgi_stdio.h"
#include "clog.h"

void file_to_stdout(const char *file);

int main() {
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n"
                "\r\n");
        generate_entries(10, 1, "html.ct");
    }

    return 0;
}

void file_to_stdout(const char *file) {
    FILE *fp;
    char c;
    fp = fopen(file,"r");
    while((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    fclose(fp);
}
