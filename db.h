#ifndef __CLOG_DB__
#define __CLOG_DB__

#include <sqlite3.h>
#include <stdio.h>
#include <time.h>

#define QUERY_PAD   (32)  /* extra room for query keywords    */
#define SYMBOL_PAD   (4)  /* extra room for quotes and commas */
#define RFCDATE_PAD (32)  /* extra room for rfc date string   */

typedef sqlite3 clog_db;

typedef enum {
	CLOG_DB_INSERT,
	CLOG_DB_UPDATE,
	CLOG_DB_DELETE
} mod_type;

struct clog_transaction {
	mod_type type;     /* transaction type                   */
	unsigned int id;   /* only used in UPDATE/DELETE         */
	char *title;       /* pointer to title buffer            */
	char *content;     /* pointer to content buffer          */
	time_t time;       /* c_time or u_time depending on type */
};

/* clog_db open/close functions */
clog_db *clog_db_open(const char *name);
void clog_db_close(clog_db *db);

/* clog_db manipulation functions */
int 
clog_modify_db(
	clog_db *database_handle, 
	struct clog_transaction *transaction);


#endif /* __CLOG_DB__ */
