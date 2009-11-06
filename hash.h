#ifndef _HASH_H_
#define _HASH_H_

/* hash table for storing parsed column data and later merging it into a template file
 * Example:
 *   if sqlite resulted in a array with following data via sqlite3_get_table function
 *     id | name
 *     ---+-----
 *      1 | Tim
 *      2 | Bob
 *
 *   then you may store this data in this hash table by
 *
 *   t = new_table(results);  where results was the 3rd param of sqlite3_get_table function
 *
 *   after, you can apply template later using functions in template.h
 *
 *
 *   err... maybe this is a bad design
 */

#define BUCKET_SIZE 10

//if you need to parse the value before printing
typedef void printer(char *, const char *);

typedef struct hash {
    char *key;
    char *val;
    printer *print_f;
    struct hash *next; //for collision
} hash_t;

typedef hash_t *hash_table[BUCKET_SIZE];

void hash_table_init(hash_table h);

/* only frees the structs not the strings
 * you do that yourself! 
 * so DO NOT call this unless you have already manually removed all the strings
 * if you are using clog, then just call sqlite3_free_table */
void hash_free(hash_table h);

/* this function does NOT copy *key, *val
 * so, once you are done with the hash table
 * your have to free those strings!
 *  if you are using this in the context of clog
 *  you can just use sqlite3_free_table */
void hash_add(hash_table h, char *key, char *val, printer *f);

void hash_print(hash_table table, char *key, const char *param);

void hash_print_all(hash_table table);

hash_t *hash_find(hash_table table, char *key);

int hasher(char *key);


#endif
