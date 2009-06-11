#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hash.h"



void emph(char *text);
void test_hash();
void assert_hash_table(hash_table h);

int main() {

    return 0;
}

void emph(char *text) {
    printf("%s!", text);
}

void test_hash() {
    int i;
    hash_table h;

    hash_table_init(h);
    printf("> Testing hash_table_init(hash_table)...\n");
    for (i = 0; i < BUCKET_SIZE; i++) {
        assert(h[i] == NULL);
    }
    printf(">>> hast_table_init(hash_table) passed!\n");

    printf("\n");

    printf("> Testing hasher(char *)...\n");
    assert(hasher("a") == ((int)'a' % BUCKET_SIZE));
    assert(hasher("abc") == (((int)'a' + (int)'b' + (int)'c') % BUCKET_SIZE));
    printf(">>> hasher(char *) passed!\n");

    printf("\n");

    printf("> Testing hash_add...\n");
    hash_add(h, "name", "Tim", emph);
    assert(h[hasher("name")]->key == "name");
    assert(h[hasher("name")]->val == "Tim");
    assert(h[hasher("name")]->next == NULL);
    assert(h[hasher("name")]->print_f == &emph);

    hash_add(h, "a", "123", NULL);
    assert(h[hasher("name")]->next != NULL);
    assert(h[hasher("name")]->next->key == "a");
    assert(h[hasher("name")]->next->val == "123");
    assert(h[hasher("name")]->next->next == NULL);
    assert(h[hasher("name")]->next->print_f == NULL);
    printf(">>> hash_add passed!\n");


    printf("\n");

    printf("> Testing hash_print_all...\n");
    hash_print_all(h);
    assert_hash_table(h);
    printf(">>> hash_print_all passed!\n");

    printf("\n");

    printf("> Testing hash_free...\n");
    hash_free(h);
    printf(">>> hash_free passed!\n");
}

void assert_hash_table(hash_table h) {
    int i;
    for (i = 0; i < BUCKET_SIZE; i++) {
        if (i != hasher("name")) {
            assert(h[i] == NULL);
        }
    }
    assert(h[hasher("name")]->key == "name");
    assert(h[hasher("name")]->val == "Tim");
    assert(h[hasher("name")]->print_f == &emph);
    assert(h[hasher("name")]->next != NULL);
    assert(h[hasher("name")]->next->key == "a");
    assert(h[hasher("name")]->next->val == "123");
    assert(h[hasher("name")]->next->print_f == NULL);
    assert(h[hasher("name")]->next->next == NULL);
}


