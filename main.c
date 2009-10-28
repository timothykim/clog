#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fcgi_stdio.h"
#include "clog.h"

void get_param(char *q_str, char *search, char *ret, size_t n);

int main() {
    while (FCGI_Accept() >= 0) {
        char type[80] = "html";
        char entry[10] = "0";
        char comments[6] = "false";

        if(getenv("QUERY_STRING") != NULL) {
            char gets[1000];

            strcpy(gets, getenv("QUERY_STRING"));
            get_param(gets, "entry", entry, 10);

            strcpy(gets, getenv("QUERY_STRING"));
            get_param(gets, "format", type, 20);

            strcpy(gets, getenv("QUERY_STRING"));
            get_param(gets, "get_comments", comments, 6);
        }


        if (strcmp(type, "html") == 0) {
            // application/xhtml+xml doesn't work with IE -_-;;
            //printf("Content-type: application/xhtml+xml; charset=UTF-8\r\n"
            // have to serve xhtml as html... IE sucks
            printf("Content-type: text/html; charset=UTF-8\r\n"
                    "\r\n");
        } else
        if (strcmp(type, "json") == 0) {
            printf("Content-type: application/json; charset=UTF-8\r\n"
                    "\r\n");
        } else
        if (strcmp(type, "rss") == 0) {
            printf("Content-type: application/rss+xml; charset=UTF-8\r\n"
                    "\r\n");
        } else 
        if (strcmp(type, "atom") == 0) {
            printf("Content-type: application/atom+xml; charset=UTF-8\r\n"
                    "\r\n");
        } else {
            printf("Status: 400 Bad Request\r\n\r\n");
            break;
        }

        if (strcmp(comments, "true") == 0) {
            strcat(type, "_comments.ct");
            generate_comments(strtol(entry, NULL, 10), type);
        } else {
            /* defaults to creating the main blog */
            strcat(type, ".ct");
            if(strcmp(entry,"0") == 0) {
                generate_entries(10, 1, type);
            } else { 
                generate_entries(1, strtol(entry, NULL, 10), type);
            }
        }
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
