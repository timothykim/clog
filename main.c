#include <stdio.h>
#include <stdlib.h>
#include "fcgi_stdio.h"
#include "clog.h"

void file_to_stdout(const char *file);

int main() {
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n"
                "\r\n");

        file_to_stdout("head.html");
        
        print_entries(2, 1, 
            "<div class=\"entry\" id=\"entry%s\">\n"
            "<h2 class=\"title\">%s</h2>\n"
            "<div class=\"datetime\">\n%s\n</div>\n"
            "<div class=\"content\">%s</div>\n"
            "</div>\n"
        );

        file_to_stdout("foot.html");
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
