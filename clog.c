#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "clog.h"

int db_execute_query(char *q) {
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


int add_entry(char *title, char *content) {
	char *q;

	/* TODO: make sure the q size is correct including time() */
	q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

	sprintf(q, "INSERT INTO entries (title, content, c_time) "
			   "VALUES ( \"%s\", \"%s\", %d);",
			   clean_up(title), clean_up(content), time(NULL));

	free(q);

	return db_execute_query(q);
}

int update_entry(int id, char *title, char *content) {
	char *q;

	/* TODO: make sure the q size is correct including id */
	q = (char *)malloc(sizeof(char) * (70 + strlen(title) + strlen(content)));

	sprintf(q, "UPDATE entries SET "
			   "title=\"%s\", "
			   "content=\"%s\" "
			   "u_time=%d"
			   "WHERE id=%d;",
			   title, content, time(NULL), id);

	free(q);

	return db_execute_query(q);
}

int delete_entry(int id) {
	char *q;

	/* TODO: make sure the q size is correct with the id */
	q = (char *)malloc(sizeof(char) * 70);

	sprintf(q, "UPDATE entries SET deleted=1 WHERE id=%d;", id);

	return db_execute_query(q);
}

entry_t **get_entries(int window, int start_t) {

	return NULL;
}


