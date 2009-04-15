#ifndef _CLOG_H_
#define _CLOG_H_

#include <sqlite3.h>

#define DATABASE "clog.db"

#define CLOG_ID  row * c
#define CLOG_TITLE row * c + 1
#define CLOG_CONTENT row * c + 2
#define CLOG_C_TIME row * c + 3
#define CLOG_U_TIME row * c + 4

/* sqlite3 query function wrapper */
int db_modify_table(const char *);

/* add_entry(title, content) */
int add_etnry(const char *, const char *);

/* update_entry(id, title, content) */
int update_entry(int, const char *, const char *);

/*
 * 3rd parameter is the template text for the single row output
 */
int print_entries(int, int, const char *);

/* remove_entry(id) */
int remove_entry(int);

#endif
