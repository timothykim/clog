#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }

    sqlite3_close(db);
    return 0;
}


int add_entry(const char *title, const char *content) {
    char *q;

    /* TODO: make sure the q size is correct including time() */
    q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

    sprintf(q, "INSERT INTO entries (title, content, c_time) "
               "VALUES ( \"%s\", \"%s\", %d);",
               (title), (content), time(NULL));

    return db_modify_table(q);
}

int update_entry(int id, const char *title, const char *content) {
    char *q;

    /* TODO: make sure the q size is correct including id */
    q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

    sprintf(q, "UPDATE entries SET "
               "title=\"%s\", "
               "content=\"%s\" "
               "u_time=%d"
               "WHERE id=%d;",
               title, content, time(NULL), id);

    return db_modify_table(q);
}

int delete_entry(int id) {
    char *q;

    /* TODO: make sure the q size is correct with the id */
    q = (char *)malloc(sizeof(char) * 70);

    sprintf(q, "UPDATE entries SET deleted=1 WHERE id=%d;", id);

    return db_modify_table(q);
}

int print_entries(int window, int id, const char *template) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *zErrMsg = 0;
    char *q;
    char **results = 0;
    int column, c;
    int row, r;

    q = (char *)malloc(sizeof(char) * 70);
    sprintf(q, "SELECT "
               "id, title, content, datetime(c_time, 'unixepoch'), datetime(u_time, 'unixepoch') "
               "FROM entries "
               "WHERE id >= %d AND deleted = 0 "
               "LIMIT %d ",
               id, window);

    rc = sqlite3_open(DATABASE, &db);
    if(rc) {
        /* error */
        return rc;
    }

    rc = sqlite3_get_table(db, q, &results, &r, &c, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }


    for (row = 1; row <= r; row++) {
        printf("%s\t%s\t%s\n", results[CLOG_ID], results[CLOG_TITLE], results[CLOG_CONTENT]);

    }

    sqlite3_free_table(results);

    sqlite3_close(db);
    return 0;
}

