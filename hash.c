#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"


void hash_table_init(hash_table h) {
    int i;
    for (i = 0; i < BUCKET_SIZE; i++) {
        h[i] = NULL;
    }
}

void hash_free(hash_table h) {
    int i;
    hash_t *curr;
    hash_t *prev;
    for (i = 0; i < BUCKET_SIZE; i++) {
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
}

void hash_add(hash_table h, char *key, char *val, printer *f) {
    int k = hasher(key);

    // we don't do duplicate key check!

    //create the new hash item
    hash_t *new;
    new = (hash_t *)malloc(sizeof(hash_t));
    new->key = key;
    new->val = val;
    new->print_f = f;
    new->next = NULL;

    //if no collision
    if (h[k] == NULL) {
        h[k] = new;
        return;
    }

    //collision! T_T
    hash_t *curr = h[k];

    while(curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new;

    return;
}


void hash_print(hash_table table, char *key) {
    hash_t *h = hash_find(table, key);
    if (h == NULL) {
        return;
    } else if (h->print_f == NULL) {
        if (h->val != NULL) {
            printf("%s", h->val);
        }
    } else {
        h->print_f(h->val);
    }
}

void hash_print_all(hash_table table) {
    int i;
    hash_t *curr;
    printf("\n\n");
    for (i = 0; i < BUCKET_SIZE; i++) {
        curr = table[i];
        while (curr != NULL) {
            printf(" %s\t\t| %s\t\t| ", curr->key, curr->val);
            if (curr->print_f != NULL) {
                curr->print_f(curr->val);
            }
            printf("\n");
            curr = curr->next;
        }
    }
    printf("\n");
}

hash_t *hash_find(hash_table table, char *key) {
    int k = hasher(key);
    hash_t *curr = table[k];
    while (curr != NULL && strcmp(curr->key, key) != 0) {
        curr = curr->next;
    }
    return curr;
}


int hasher(char *key) {
    int i;
	int sum = 0;
	for (i = 0; i < strlen(key); i++) {
		sum += (int)key[i];
	}
	return sum % BUCKET_SIZE;
}
