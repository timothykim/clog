#ifndef __CLOG_DB__
#define __CLOG_DB__

#include <sqlite3.h>
#include <stdio.h>
#include <time.h>

#define QUERY_PAD   (32)
#define SYMBOL_PAD   (4)
#define RFCDATE_PAD (32)

typedef sqlite3 clog_db;

typedef enum {
	CLOG_DB_INSERT,
	CLOG_DB_UPDATE,
	CLOG_DB_DELETE
} mod_type;

struct clog_transaction {
	mod_type type;
	unsigned int id;
	char *title;
	char *content;
	time_t time;
};

clog_db *clog_db_open(const char *name);
void clog_db_close(clog_db *db);

int 
clog_modify_db(
	clog_db *database_handle, 
	struct clog_transaction *transaction);


#endif /* __CLOG_DB__ */
