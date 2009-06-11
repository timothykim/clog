#ifndef _CLOG_H_
#define _CLOG_H_

#include <sqlite3.h>
#include <errno.h>
#include "hash.h"

#define DATABASE "clog.db"

#define CLOG_ID  row * c
#define CLOG_TITLE row * c + 1
#define CLOG_CONTENT row * c + 2
#define CLOG_C_TIME row * c + 3
#define CLOG_U_TIME row * c + 4
#define CLOG_COMMENT_COUNT row * c + 5

#define CLOG_TYPE_JSON 0
#define CLOG_TYPE_RSS_2_0 1
#define CLOG_TYPE_HTML 2

#define CLOG_SCRIPT_OPEN "{="
#define CLOG_SCRIPT_CLOSE "=}"

#define CLOG_LOOP_BEGIN "{=loop_begin=}\n"
#define CLOG_LOOP_END "{=loop_end=}\n"

#define CLOG_BEFORE_LOOP 0
#define CLOG_INSIDE_LOOP 1
#define CLOG_AFTER_LOOP 2

#define FILE_BUFFER_SIZE 100

#ifndef HAVE_ERRNO_DEF
extern  int     errno;
#endif


typedef struct entry {
    int id;
    char *title;
    char *content;
    char *c_time;
    char *u_time;
    int comment_count;
} entry_t;


/* sqlite3 query function wrapper */
int db_modify_table(const char *);

int print_template(const char *, hash_table[], int);

/* add_entry(title, content) */
int add_entry(const char *, const char *);

/* update_entry(id, title, content) */
int update_entry(int, const char *, const char *);

/*
 *  generate_entries(# of entries, starting entry, template file)
 */
int generate_entries(int, int, const char *);
int free_entries(entry_t entries[], int count);

/* generate_comments(entry, template file) */
int generate_comments(int, const char *);

/* remove_entry(id) */
int remove_entry(int);

/*
 * prints str where two or more \n are converted to <p> 
 */
void htmlize_print(char *str);
void print_comment_count(char *count);

int error_log(const char *fmt, ...);

void rfc_date(char *d_str, time_t time);

#endif
