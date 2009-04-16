#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fcgi_stdio.h"
#include "clog.h"

void get_param(char *q_str, char *search, char *ret, size_t n);

int main() {
    char type[20] = "html";

    while (FCGI_Accept() >= 0) {
        if(getenv("QUERY_STRING") != NULL) {
            char gets[1000];
            strcpy(gets, getenv("QUERY_STRING"));
            get_param(gets, "format", type, 20);
        }


        if (strcmp(type, "html") == 0) {
            printf("Content-type: application/xhtml+xml; charset=UTF-8\r\n"
                    "\r\n");
        } else
        if (strcmp(type, "json") == 0) {
            printf("Content-type: application/json; charset=UTF-8\r\n"
                    "\r\n");
        } else
        if (strcmp(type, "rss") == 0) {
            printf("Content-type: application/rss+xml; charset=UTF-8\r\n"
                    "\r\n");
        } else {
            printf("Status: 400 Bad Request\r\n\r\n");
            return 0;
        }

        strcat(type, ".ct");
        generate_entries(10, 1, type);

    }

    return 0;
}

void get_param(char *q_str, char *search, char *ret, size_t n) {
    char *sep = "&";
    char *sep2 = "=";
    char *query, *tokptr;
    char *key, *val, *tokptr2;

    for(query = strtok_r(q_str, sep, &tokptr);
        query != NULL;
        query = strtok_r(NULL, sep, &tokptr)) {

        key = strtok_r(query, sep2, &tokptr2);
        val = strtok_r(NULL, sep2, &tokptr2);
        
        if (strcmp(key, search) == 0) {
            if (strlen(val) >= n) {
                strcpy(ret, "bad");
                return;
            }
            strcpy(ret, val);
            return;
        }
    }
}

/*
int get_query(char *ret, size_t n, char *query_string, char *search_string) {
    char sep1[2] = "&";
    char sep2[2] = "=";
    char *query, *tok1, *tok2;
    char *key, *value;

    printf("%s\n", query_string);

    for (query = strtok_r(query_string, "&", &tok1);
         query;
         query = strtok_r(NULL, "&", &tok1)) {
        
        key = strtok_r(query, sep2, &tok2);
        value = strtok_r(NULL, sep2, &tok2);

        if (strcmp(key, search_string) == 0) {
            if (strlen(value) >= n) {
                return -1; //not bigenough
            }
            strcpy(ret, value);
            return 0;
        }
    }

    return 0;
}
*/
