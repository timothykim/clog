#ifndef _CLOG_H_
#define _CLOG_H_


#define DATABASE "clog.db"

typedef struct entry {
	char *title;
	char *content;
	int timestamp;
} entry_t;

/* add_entry(title, content) */
int add_etnry(char *, char *);

/* update_entry(id, title, content) */
int update_entry(int, char *, char *);

/*
 * get_entries(latest_n, on_and_after_id) 
 * returns array of pointers to entry
 */
entry_t **get_entries(int, int);

/* remove_entry(id) */
int remove_entry(int);

#endif
