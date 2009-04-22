#ifndef __CLOG_DB__
#define __CLOG_DB__

#include <stdio.h>
#include <time.h>

#define QUERY_PAD   (32)
#define SYMBOL_PAD   (4)
#define RFCDATE_PAD (32)

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

#endif /* __CLOG_DB__ */
