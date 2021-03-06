#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include "hash.h"
#include "clog.h"

int db_modify_table(const char *q) {
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;

    rc = sqlite3_open(DATABASE, &db);
    if(rc) {
        /* error */
        return -rc;
    }

    FILE *f;
    char date[50];
    format_date(date, time(NULL), "");
    f = fopen("query.log", "w");
    fprintf(f, "\n%s - %s", date, q);

    rc = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(f, "\n\nIn db_modify_table() SQL error: %s\n", zErrMsg);
        error_log("In db_modify_table() SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -rc;
    }

    if (strncmp(q, "INSERT", 6) == 0) {
        rc = sqlite3_last_insert_rowid(db);
    }


    sqlite3_close(db);

    fclose(f);

    return rc;
}

int add_comment(int entry_id, const char *comment) {
    char *q;
    int rs, u;

    q = sqlite3_mprintf("INSERT INTO comments (entry_id, comment, c_time) "
                        "VALUES ( %d, '%q', %d); ",
                        (entry_id), comment, (int)time(NULL));
                        /*
                        "UPDATE entries SET "
                        "comment_count=commnet_count+1 "
                        "WHERE id=%d;",
                        (entry_id), (comment), (int)time(NULL), entry_id);
                        */
    rs = db_modify_table(q);
    sqlite3_free(q);

    return rs;
}

int add_entry(const char *title, const char *content) {
    char *q;
    int rs;
	sqlite3 *db;

    q = sqlite3_mprintf("INSERT INTO entries (title, content, c_time) "
                        "VALUES ( '%q', '%q', %d);",
                        (title), (content), (int)time(NULL));

    rs = db_modify_table(q);

    sqlite3_free(q);

	return rs;
}

int update_entry(int id, const char *title, const char *content) {
    char *q;
    int rs;

    /* TODO: make sure the q size is correct including id */
    q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

    q = sqlite3_mprintf("UPDATE entries SET "
               "title='%q', "
               "content='%q', "
               "u_time=%d "
               "WHERE id=%d;",
               title, content, (int)time(NULL), id);

    rs = db_modify_table(q);

    sqlite3_free(q);

    return rs;
}


int print_template(const char *template_file, hash_table h[], int count) {
    FILE *fc;
    int i;
    int key_size;
    char buffer[FILE_BUFFER_SIZE];
    char *pt;
    char *key;
    char *open;
    char *close;
    char *loop_str;
    char *param;
    char *file_name;
    FILE *file_ptr;
    loop_str = (char *)malloc(sizeof(char) * 1);
    loop_str[0] = 0;

    fc = fopen(template_file, "r");
    if (fc == NULL) {
        error_log("Failed opening template file: %s\n", template_file);
        return errno;
    }

    /* read in the template and just output the buffer */
    while (fgets(buffer, sizeof(buffer), fc)) {
        /* file include */
        if (strncmp(buffer, CLOG_FILE_INCLUDE, 7) == 0) {
            buffer[strlen(buffer)-3] = 0;
            file_name = buffer+strlen(CLOG_FILE_INCLUDE);
            file_ptr = fopen(file_name, "r");
            if (file_ptr==NULL) {
                printf("<!-- Error: can't open file. -->\n");
            } else {
                while (fgets(buffer, sizeof(buffer), file_ptr)) {
                    printf("%s", buffer);
                }
                fclose(file_ptr);
            }
            continue;
        }

        /* if we hit the loop parse and output */
        if (strcmp(buffer, CLOG_LOOP_BEGIN) == 0) {
            /* build up the loop template */
            while (fgets(buffer, sizeof(buffer), fc)) {
                /* we hit the end of the loop */
                if (strcmp(buffer, CLOG_LOOP_END) == 0) {
                    break;
                } else { /* else update the loop string */
                    loop_str = (char *)realloc(loop_str, sizeof(char) * (strlen(loop_str) + strlen(buffer) + 1));
                    strcat(loop_str, buffer);
                }
            }

            /* loop through the hash table array to print out the data */
            for (i = 0; i < count; i++) {
                pt = loop_str;
                while(*pt) {
                    if (strncmp(pt, CLOG_SCRIPT_OPEN, strlen(CLOG_SCRIPT_OPEN)) == 0) {
                        open = pt + strlen(CLOG_SCRIPT_OPEN);
                        close = strstr(open, CLOG_SCRIPT_CLOSE);
                        pt = close + strlen(CLOG_SCRIPT_CLOSE);
                        key_size = close - open;
                        key = (char *)malloc(sizeof(char) * (key_size + 1));
                        strncpy(key, open, key_size);
                        key[key_size] = 0;
                        //if key contains a space that means we have a parameter!
                        param = key;
                        while (*param) {
                            param++;
                            if (*param == ' ') {
                                *param = 0;
                                param++;
                                break;
                            }
                        }

                        if (strcmp(key, "i") == 0) {
                            printf("%d",i);
                        } else {
                            hash_print(h[i], key, param);
                        }
                        free(key);
                    }
                    printf("%c", *pt);
                    pt++;
                }
            }
            free(loop_str);
            loop_str = (char *)malloc(sizeof(char) * 1);
            loop_str[0] = 0;
            continue; /* keep the CLOG_LOOP_END from priting */
        }
        printf("%s", buffer);
    }
    free(loop_str);

    return 0;
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
    int col, c; // number of columns returned from query
    int row, r; // r: number of rows returned from query
    //int i; //generic counter (DO NOT USE IT FOR STORAGE)
    void *pfptr;

    q = (char *)malloc(sizeof(char) * 250);

    sprintf(q, "SELECT "
            "id, title, content, "
            "c_time, "
            "u_time, "
            "comment_count "
            "FROM entries_with_comment_count "
            "WHERE id %s %d "
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
    free(q);

    hash_table entries[r];

    int index;
    for (row = 1; row <= r; row++) {
        index = row - 1;

        hash_table_init(entries[index]);
        for (col = 0; col < c; col++) {
            pfptr = NULL;
            if (strcmp(results[col], "content") == 0) {
                pfptr = htmlize_print;
            } else
            if (strcmp(results[col], "comment_count") == 0) {
                pfptr = print_comment_count;
            } else
            if (strcmp(results[col], "c_time") == 0) {
                pfptr = print_date;
            } else
            if (strcmp(results[col], "u_time") == 0) {
                pfptr = print_date;
            }
            hash_add(entries[index], results[col], results[row * c + col], pfptr);
        }
    }

    print_template(template_file, entries, r);

    /*
	 * TODO: fix this :(
    for (i = 0; i < r; i++) {
        hash_free(entries[r]);
    }
    */
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


int generate_comments(int entry_id, const char *template_file) {
    sqlite3 *db;

    char *zErrMsg = 0;
    int rc;
    char *q;
    char **results = 0;
    int r, c, col, row;
    void *pfptr;

    q = (char *)malloc(sizeof(char) * 250);

    sprintf(q, "SELECT "
               "* "
               "FROM comments "
               "WHERE entry_id = %d " 
               "ORDER BY c_time ASC ",
               entry_id);

    if((rc = sqlite3_open(DATABASE, &db)))
        return rc;
    
    rc = sqlite3_get_table(db, q, &results, &r, &c, &zErrMsg);
    if (rc != SQLITE_OK) {
        error_log("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    }
    
    hash_table comments[r];

    int index;
    for (row = 1; row <= r; row++) {
        index = row - 1;

        hash_table_init(comments[index]);
        for (col = 0; col < c; col++) {
            pfptr = NULL;
            if (strcmp(results[col], "comment") == 0) {
                //pfptr = print_comment_count;
            } else
            if (strcmp(results[col], "c_time") == 0) {
                pfptr = print_date;
            }
            hash_add(comments[index], results[col], results[row * c + col], pfptr);
        }
    }

    print_template(template_file, comments, r);

    free(q);
    sqlite3_free_table(results);
    sqlite3_close(db);
    return 0;
}

void htmlize_print(char *str, const char *param) {
    char *sep = "\n\r";
    char *paragraph;

    int do_not_format = 0;

    paragraph = strtok(str, sep);

    if (strstr(paragraph, "<pre")) {
        do_not_format = 1;
    }

    //don't do formatting... 
    do_not_format = 1;

    while(paragraph != NULL) {
        if (do_not_format) {
            printf("%s\n", paragraph);
        } else {
            printf("<p>%s</p>\n\n", paragraph);
        }

        paragraph = strtok(NULL, sep);

        if (paragraph) {
            if (strstr(paragraph, "<pre")) {
                do_not_format = 1;
            }
            if (strstr(paragraph,"</pre>")) {
                printf("%s\n", paragraph);
                paragraph = strtok(NULL, sep);
                do_not_format = 0;
            }
        }
    }
}


void print_hash_tables(char *filename, hash_table tables[], int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        hash_print_all(tables[i]);
    }
}

/* TODO: write the error to the file */
int error_log(const char *fmt, ...) {
    int ret;
    va_list ap;

    /*
    FILE *f;
    char date[50];
    format_date(date, time(NULL), "");

    f = fopen("error.log", "a");
    fprintf(f, "\n%s - ", date);

    */

    va_start(ap, fmt);
    ret = fprintf(stderr, fmt, ap);
    va_end(ap);

    //fclose(f);

    return (ret);
}

void print_comment_count(char *count, const char *param) {
    if(strcmp(count, "0") != 0) {
        printf(" (%s)", count);
    }
}

void print_date(char *time, const char *format) {
    if (time != NULL) {
        char date[50];
		//const char *format = "%a, %d %b %Y %H:%M:%S %z";
		//const char *format = "%Y-%m-%dT%H:%M:%SZ";
        if (strlen(format) == 0) {
            format_date(date, strtol(time, NULL, 10), "%a, %d %b %Y %H:%M:%S %z");
        } else {
            format_date(date, strtol(time, NULL, 10), format);
        }
        printf("%s", date);
    }
}

void format_date(char *d_str, time_t time, const char *format) {
    struct tm *date;

    date = gmtime((const time_t *)&time);
    (void)strftime(d_str, 32, format, date);
}
