#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include "clog.h"

int db_modify_table(const char *q) {
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;

    rc = sqlite3_open(DATABASE, &db);
    if(rc) {
        /* error */
        return rc;
    }

    rc = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        error_log("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }

    sqlite3_close(db);
    return 0;
}


int add_entry(const char *title, const char *content) {
    char *q;
    int rs;

    /* TODO: make sure the q size is correct including time() */
    q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

    sprintf(q, "INSERT INTO entries (title, content, c_time) "
               "VALUES ( \"%s\", \"%s\", %d);",
               (title), (content), (int)time(NULL));
    rs = db_modify_table(q);
    free(q);

    return rs;
}

int update_entry(int id, const char *title, const char *content) {
    char *q;
    int rs;

    /* TODO: make sure the q size is correct including id */
    q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

    sprintf(q, "UPDATE entries SET "
               "title=\"%s\", "
               "content=\"%s\" "
               "u_time=%d"
               "WHERE id=%d;",
               title, content, (int)time(NULL), id);

    rs = db_modify_table(q);
    free(q);

    return rs;
}

int remove_entry(int id) {
    char *q;
    int rs;

    /* TODO: make sure the q size is correct with the id */
    q = (char *)malloc(sizeof(char) * 70);

    sprintf(q, "UPDATE entries SET deleted=1 WHERE id=%d;", id);

    rs = db_modify_table(q);
    free(q);

    return rs;
}


int generate_entries(int window, int id, const char *template_file) {
    sqlite3 *db;
    // sqlite3_stmt *stmt;
    int rc;
    char *zErrMsg = 0;
    char *q;
    char **results = 0;
    int c; // number of columns returned from query
    int row, r; // r: number of rows returned from query
    FILE *fc;
    char buffer[FILE_BUFFER_SIZE];
    char *loop_str;
    int ts; /* keep track of which part of the template file we are in */
    char e_str[1];
    e_str[0] = 0;

    q = (char *)malloc(sizeof(char) * 250);

    sprintf(q, "SELECT "
            "id, title, content, "
            "c_time, "
            "u_time "
            "FROM entries "
            "WHERE id %s %d AND deleted = 0 "
            "ORDER BY c_time DESC "
            "LIMIT %d ",
            (window == 1) ? "=" : ">=",
            id, window);


    rc = sqlite3_open(DATABASE, &db);

    if(rc) { //error
        return rc;
    }

    rc = sqlite3_get_table(db, q, &results, &r, &c, &zErrMsg);
    if (rc != SQLITE_OK) {
        error_log("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }

    entry_t entries[r];

    int index;
    for (row = 1; row <= r; row++) {
        index = row - 1;
        entries[index].id = strtol(results[CLOG_ID], NULL, 10);
        entries[index].title   = results[CLOG_TITLE];
        entries[index].content = results[CLOG_CONTENT];

        entries[index].c_time = (char *)malloc(sizeof(char) * 32);
        entries[index].u_time = (char *)malloc(sizeof(char) * 32);

        if (results[CLOG_C_TIME] == NULL) {
            entries[index].c_time = NULL;
        } else {
            rfc_date(entries[index].c_time, strtol(results[CLOG_C_TIME], NULL, 10));
        }

        if (results[CLOG_U_TIME] == NULL) {
            entries[index].u_time = NULL;
        } else {
            rfc_date(entries[index].u_time, strtol(results[CLOG_U_TIME], NULL, 10));
        }
    }


    fc = fopen(template_file, "r");
    if (fc == NULL) {
        error_log("Failed opening template file: %s\n", template_file);
        return errno;
    }

    ts = CLOG_BEFORE_LOOP;
    loop_str = (char *)malloc(sizeof(char));
    *(loop_str) = 0;

    while (fgets(buffer, sizeof(buffer), fc)) {
        if (strcmp(buffer, CLOG_LOOP_BEGIN) == 0) {
            ts = CLOG_INSIDE_LOOP;
            continue;
        }

        if (strcmp(buffer, CLOG_LOOP_END) == 0) {
            /* now that we ended the loop parse the loop_str for each entries */
            //printf("%s", loop_str);
            for (row = 0; row < r; row++) {
                output_entry(loop_str, entries[row]);
            }
            ts = CLOG_AFTER_LOOP;
            continue;
        }

        switch (ts) {
            case CLOG_INSIDE_LOOP:
                /*
                if (sizeof(loop_str) < (sizeof(char) * (strlen(loop_str) + strlen(buffer) + 1))) {
                    loop_str = (char *)realloc(loop_str, sizeof(char) * 2 * (sizeof(loop_str) + strlen(buffer) + 1));
                }
                */
                loop_str = (char *)realloc(loop_str, sizeof(char) * (strlen(loop_str) + strlen(buffer) + 1));
                strcat(loop_str, buffer);
                break;
            default:
                printf("%s", buffer);
                break;
        }
    }

    free(loop_str);

    free_entries(entries, r);

    sqlite3_free_table(results);

    sqlite3_close(db);
    return 0;
}

int free_entries(entry_t entries[], int count) {
    int i;
    for (i = 0; i < count; i++) {
        free(entries[i].c_time);
        free(entries[i].u_time);
    }
	return 0;
}

void output_entry(char *tmplate, const entry_t e) {
    char *pt;
    char tag[20];
    int i;

    pt = tmplate;

    while(*pt) {
        if ((*pt) == '{' && (*(pt+1)) == '=') {
            pt = pt + 2;
            i = 0;
            while((*pt) != '=' && (*(pt+1)) != '}' && i < 20) {
                tag[i] = *pt;
                pt++;
                i++;
            }
            tag[i] = 0;
            pt++;

            if (strcmp(tag, "id") == 0) printf("%d", e.id);
            if (strcmp(tag, "title") == 0 && e.title != NULL) printf("%s", e.title);
            if (strcmp(tag, "content") == 0 && e.content != NULL) htmlize_print(e.content);
            if (strcmp(tag, "c_time") == 0 && e.c_time != NULL) printf("%s", e.c_time);
            if (strcmp(tag, "u_time") == 0 && e.u_time != NULL) printf("%s", e.u_time);
        } else {
            printf("%c", *pt);
        }
        pt++;
    }
}

void htmlize_print(char *str) {
    char *sep = "\n";
    char *paragraph;

    paragraph = strtok(str, sep);
    while(paragraph != NULL) {
        printf("<p>%s</p>\n\n", paragraph);
        paragraph = strtok(NULL, sep);
    }
}

int error_log(const char *fmt, ...) {
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = fprintf(stderr, fmt, ap);
    va_end(ap);
    return (ret);
}

void rfc_date(char *d_str, time_t time) {
    struct tm *date;
    const char *format = "%a, %d %b %Y %H:%M:%S %z";

    date = localtime((const time_t *)&time);
    (void)strftime(d_str, 32, format, date);
}
