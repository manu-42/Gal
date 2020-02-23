#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pile.h"

PILE create_PILE() {
    return NULL;
}

int isEmpty(PILE p) {
    return p == NULL;
}

void push(PILE *p, void *val) {
    MAILLON *m = malloc(sizeof(MAILLON));
    m->val = val;
    m->next = *p;
    *p = m;
}

void push_char(PILE *p, char ch) {
    char *val = malloc(sizeof(char));
    *val = ch;
    push(p, val);
}

void *pop(PILE *p) {
    assert(!isEmpty(*p));
    void *val = (*p)->val;
    PILE p_old = *p;
    *p = (*p)->next;
    free(p_old);
    return val;
}

char pop_char(PILE *p) {
    void *v = pop(p);
    char ch = *((char*) v);
    free(v);
    return ch;
}

void *sommet(PILE p) {
    assert(!isEmpty(p));
    return p->val;
}

char sommet_char(PILE p) {
    return *((char*) sommet(p));
}
