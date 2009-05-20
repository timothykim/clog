#include <stdlib.h>
#include <stdio.h>
#include "hash.h"


void emph(char *text) {
    printf("%s!\n", text);
}

int main() {
    hash_table h;
    hash_table_init(h);
    hash_add(h, "name", "Tim", emph);
    hash_print_all(h);
    hash_free(h);
    return 0;
}
