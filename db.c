#include <stdlib.h>
#include <time.h>

#include "db.h"


// compute nearest power of two that is
// greater than or equal to n
inline int pow2(int n)
{
	int i;
	for (i=1;i<n;i*=2); 
	return i;
}

sqlite3 *
clog_db_open(const char *name)
{
	sqlite3 *db;
	sqlite3_open(name, &db);
	return db;
}

void
clog_db_close(sqlite3 *db)
{
	if (!db)
		return;

	sqlite3_close(db);
}

char *
clog_allocate_query(
	struct clog_transaction *transaction,
	int *qlen)
{
	static int querylen = 0;
	static char *query = NULL;
  
	int total_length;

	if (!transaction ||
		!transaction->title ||
		!transaction->content) {
		return NULL;
	}

	total_length += strlen(transaction->title) + SYMBOL_PAD;
	total_length += strlen(transaction->content) + SYMBOL_PAD;
	total_length += RFCDATE_PAD + QUERY_PAD;
	total_length = pow2(total_length);

	if (!query) {
		query = (char *) malloc(total_length);
		querylen = total_length;
	} else if (total_length > querylen) {
		query = (char *) realloc(query, total_length);
		querylen = total_length;
	}
	
	(*qlen) = querylen;
	return query;
}

int 
clog_modify_db(
	sqlite3 *db,
	struct clog_transaction *transaction) 
{
	mod_type mtype;
	char *querystr, *zErrMsg;
	int len, qlen, rc;

	if (!transaction ||
		!transaction->title ||
		!transaction->content ||
		transaction->time <= 0) {
		return -1;
	}

	mtype = transaction->type;
	if (mtype != CLOG_DB_INSERT &&
		transaction->id < 0) {
		return -1;
	}
	
	querystr = clog_allocate_query(transaction, &qlen);

	// TODO: take care of single-quote(') and backslash (\)

	switch (mtype) {

	case CLOG_DB_INSERT:
		len = snprintf(querystr, qlen,
				"INSERT INTO entries "
				"(title, content, c_time) "
				"VALUES ('%s','%s',%d)",
				transaction->title,
				transaction->content,
				transaction->time);
		break;
		
	case CLOG_DB_UPDATE:
		len = snprintf(querystr, qlen,
				"UPDATE entries SET "
				"title = '%s',"
				"content = '%s',"
				"u_time = %d "
				"WHERE id = %d "
				"LIMIT 1",
				transaction->title,
				transaction->content,
				transaction->time,
				transaction->id);
		break;

	case CLOG_DB_DELETE:
		len = snprintf(querystr, qlen,
				"DELETE FROM entries "
				"WHERE id = %d "
				"LIMIT 1", 
				transaction->id);
		break;

	default:
		return -1;

	}

	if (len >= qlen) {
		// a return value of qlen or more means that the output was truncated. 
		// don't want truncated a query, bail out.
		return -1;
	}

    rc = sqlite3_exec(db, querystr, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        return -1;
    }

	return 0;
}


