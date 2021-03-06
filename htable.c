#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"


struct htablerec {
    int capacity;
    int num_keys;
    int *frequencies;
    char **keys;
};



htable htable_new(int capacity) {
    int i;
    htable h = emalloc(sizeof * h);
    h->capacity = capacity;
    h->num_keys = 0;
    h->frequencies = emalloc(h->capacity * sizeof h->frequencies[0]);
    h->keys = emalloc(h->capacity * sizeof h->keys[0]);
    for (i = 0; i < h->capacity; i++) {
        h->frequencies[i] = 0;
        h->keys[i] = NULL;
    }
    return h;
}


void htable_free(htable h) {
    int i;
    for (i = 0; i < h->capacity; i++) {
        free(h->keys[i]);
    }
    free(h->keys);
    free(h->frequencies);
    free(h);
}


static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}


int htable_insert(htable h, char *str) {
    unsigned int result = htable_word_to_int(str);
    int index = (result % h->capacity);
    int collisions;
    

    /* posibility 1) no string at position */
    if (h->keys[index] == NULL) {
        h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
        strcpy(h->keys[index], str);
        h->frequencies[index]++;
        h->num_keys++;
        return 1;
    }
    /* Possibility 2) same string at that position */
    else if (strcmp(h->keys[index], str) == 0) {
        h->frequencies[index]++;
        return h->frequencies[index];
    }
    /* possibility 3) wrong str at that position */
    else if (h->keys[index] != NULL && (strcmp(h->keys[index], str) != 0)) {
        /* if htable is full */
        if (h->num_keys == h->capacity) {
            return 0;
        } else {
            for (collisions = 1; collisions < h->capacity; collisions++) {
                if (h->keys[(index + 1) % h->capacity] == NULL) {
                    h->keys[(index + 1) % h->capacity] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
                    strcpy(h->keys[(index + 1) % h->capacity], str);
                    h->frequencies[(index + 1) % h->capacity] = 1;
                    h->num_keys++;
                    return 1;
                }

                else if (strcmp(h->keys[(index + 1) % h->capacity], str) == 0) {
                    h->frequencies[(index + 1) % h->capacity]++;
                    return h->frequencies[(index + 1) % h->capacity];
                }
            }
            return 0;
        }
    }
    return 0;
}



void htable_print(htable h, FILE *stream) {
    int i;
    for (i = 0; i < h->capacity; i++) {
        if (h->frequencies[i] > 0) {
            fprintf(stream, "%d coppies of %s\n", h->frequencies[i], h->keys[i]);
        }
    }
}


int htable_search(htable h, char *str) {
    int collisions = 0;
    unsigned int result = htable_word_to_int(str);
    int index = (result % h->capacity);

    while (h->keys[index] != NULL && collisions < h->capacity) {
        if (strcmp(h->keys[index], str) != 0) {
            collisions++;
            index++;
        } else {
            return h->frequencies[index];
        }
    }
    return 0;
}
